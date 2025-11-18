// standard headers
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

// allegro5 headers
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

// custom headers
#include "window_settings.c"
#include "lib/window/window.h"

// lib
#include "lib/circle/circle.h"
#include "lib/spatial_grid/spatial_grid.h"
#include "lib/collision/window_bounds_collider/window_bounds_collider.h"
#include "lib/collision/circle_collider/circle_collider.h"



// prototypes
void update_physics(SpatialGrid* grid, Circle** circles, int num_circles, Window bounds);
void grid_draw_debug(SpatialGrid* grid);

// change these for testing
static int circle_count = 100;
static int circle_min_radius = 8;
static int circle_max_radius = 20;
static int circle_max_speed = 5;
static bool draw_grid = false;

int cell_height(Window *window)
{
    int target_size = circle_max_radius * 2.5;
    int best_height = target_size;
    int best_height_diff = window->height;

    for (int divisor = 1; divisor <= window->height; divisor++)
    {
        if (window->height % divisor == 0)
        {
            int cell_h = window->height / divisor;
            int diff = abs(cell_h - target_size);
            if (diff < best_height_diff)
            {
                best_height_diff = diff;
                best_height = cell_h;
            }
        }
    }

    return best_height;
}
int cell_width(Window *window)
{
    int target_size = circle_max_radius * 2.5;
    int best_width = target_size;
    int best_width_diff = window->width;

    for (int divisor = 1; divisor <= window->width; divisor++)
    {
        if (window->width % divisor == 0)
        {
            int cell_h = window->height / divisor;
            int diff = abs(cell_h - target_size);
            if (diff < best_width_diff)
            {
                best_width_diff = diff;
                best_width = window->width / divisor;
            }
        }
    }

    return best_width;
}

int main(void)
{
    if (!al_init())
    {
        printf("couldn't initialize allegro\n");
        return 1;
    }

    if (!al_install_keyboard())
    {
        printf("couldn't initialize keyboard\n");
        return 1;
    }

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 30.0);
    if (!timer)
    {
        printf("couldn't initialize timer\n");
        return 1;
    }

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    if (!queue)
    {
        printf("couldn't initialize queue\n");
        return 1;
    }

    ALLEGRO_DISPLAY *disp = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!disp)
    {
        printf("couldn't initialize display\n");
        return 1;
    }

    if (!al_init_ttf_addon())
    {
        printf("couldn't initialize ttf addon\n");
        return 1;
    }
    if(!al_init_primitives_addon())
    {
        printf("couldn't initialize primitives addon\n");
        return 1;
    }

    // needed if we want to print text on screen
    ALLEGRO_FONT *font = al_load_ttf_font("./FiraCodeNerdFont-Regular.ttf", 24, 0);
    if (!font)
    {
        printf("couldn't initialize font\n");
        return 1;
    }

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool redraw = true;
    bool done = false;
    ALLEGRO_EVENT event;

    al_start_timer(timer);

    Window* window = malloc(sizeof(Window));
    window->width = WINDOW_WIDTH;
    window->height = WINDOW_HEIGHT;
    
    // create spatial grid
    SpatialGrid* grid = grid_create(circle_count, window->width, window->height, cell_width(window), cell_height(window));


    // create an array of circles
    Circle *circles[circle_count];

    for (int i = 0; i < circle_count; i++)
    {
        Circle* c = circle_create(i + 1, circle_min_radius, circle_max_radius);
        // calculate random starting position on the screen
        float start_x = rand() % (window->width - 100) + 50;
        float start_y = rand() % (window->height - 100) + 50;
        
        // check whether another circle exists to avoid overlap
        bool position_ok = false;
        while (!position_ok)
        {
            position_ok = true;
            for (int j = 0; j < i; j++)
            {
                Circle* other = circles[j];
                float dx = other->position[0] - start_x;
                float dy = other->position[1] - start_y;
                float distance = sqrtf(dx * dx + dy * dy);
                if (distance < (c->radius + other->radius))
                {
                    position_ok = false;
                    start_x = rand() % (window->width - 100) + 50;
                    start_y = rand() % (window->height - 100) + 50;
                    break;
                }
            }
        }
        circle_place(c, start_x, start_y, circle_max_speed);
        circles[i] = c;
        grid_insert(grid, circles[i]);
    }

    ALLEGRO_COLOR colour = al_map_rgb(255, 255, 255);

    while (!done)
    {
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            redraw = true;
            break;
        case ALLEGRO_EVENT_KEY_DOWN:
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        }

        update_physics(grid, circles, circle_count, *window);

        if (redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));

            al_draw_text(font, al_map_rgb(255, 255, 255), 320, 0, ALLEGRO_ALIGN_CENTRE, "Bounce!");

            if (draw_grid)
            {
                grid_draw_debug(grid);
            }

            for(int i = 0; i < circle_count; i++)
            {
                Circle* c = circles[i];
                circle_draw(c, true);
            }

            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}

void grid_draw_debug(SpatialGrid* grid)
{
    for (int row = 0; row < grid->rows; row++) {
        for (int col = 0; col < grid->columns; col++) {
            int x = col * grid->cell_width;
            int y = row * grid->cell_height;
            ALLEGRO_COLOR color = (grid->cells[row][col].count > 0) 
                ? al_map_rgba(255, 255, 0, 100)  // Yellow if occupied
                : al_map_rgba(50, 50, 50, 50);    // Dark gray if empty
            al_draw_rectangle(x, y, x + grid->cell_width, y + grid->cell_height, color, 1);
        }
    }
}


void update_physics(SpatialGrid* grid, Circle** circles, int num_circles, Window bounds)
{
    grid_clear(grid);
    
    // --- 1. UPDATE PHASE (Integrate Movement) ---
    // Move all circles based on their current velocities.
    for (int i = 0; i < num_circles; i++)
    {
        circle_move(circles[i]);
        grid_insert(grid, circles[i]);
    }

    // --- 2. RESOLVE PHASE (Handle All Collisions) ---
    // A. Resolve Circle-Circle Collisions
    // This resolves overlaps and applies rebound velocities for pairs.
    ccoll_rebound_velocity(grid);
    

    // B. Resolve Circle-Wall Collisions
    // This prevents spheres from getting stuck in the walls.
    for (int i = 0; i < num_circles; i++)
        wbcoll_rebound_velocity(circles[i], bounds);
    
}
