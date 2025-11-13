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
#include "lib/circle/circle.h"
#include "lib/collision/collision.h"

// prototypes
void update_physics(Circle** circles, int num_circles, Bounds bounds);

// change these for testing
static int circle_count = 20;
static int circle_radius = 10;


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

    ALLEGRO_DISPLAY *disp = al_create_display(640, 480);
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

    set_collision_object_count(circle_count);
    // create an array of circles
    Circle *circles[circle_count];

    for (int i = 0; i < circle_count; i++)
    {
        circles[i] = NULL;
        Circle* c = circle_create(i + 1, circle_radius);
        // calculate random starting position on the screen
        float start_x = rand() % (640 - 100) + 50;
        float start_y = rand() % (480 - 100) + 50;
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
                    start_x = rand() % (640 - 100) + 50;
                    start_y = rand() % (480 - 100) + 50;
                    break;
                }
            }
        }
        circle_place(c, start_x, start_y);
        circles[i] = c;
    }

    // get starting position in the top-left quadrant
    //float start_x = rand() % (640 - 100) + 50;
    //float start_y = rand() % (480 - 100) + 50;
    //circles[0] = circle_create(50.0, al_map_rgba(255, 0, 0, 0.5));
    //circle_place(circles[0], start_x, start_y);
//
    //// get starting position in the top-right quadrant
    //start_x = rand() % (640 - 100 - 320) + 320 + 50;
    //start_y = rand() % (480 - 100) + 50;
    //circles[1] = circle_create(50.0, al_map_rgba(0, 255, 0, 0.5));
    //circle_place(circles[1], start_x, start_y);
//
    //// get starting position in the bottom-left quadrant
    //start_x = rand() % (640 - 100) + 50;
    //start_y = rand() % (480 - 100 - 240) + 240 + 50;
    //circles[2] = circle_create(50.0, al_map_rgba(0, 0, 255, 0.5));
    //circle_place(circles[2], start_x, start_y);

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

        update_physics(circles, circle_count, (Bounds){WINDOW_WIDTH, WINDOW_HEIGHT});

        if (redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));

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


void update_physics(Circle** circles, int num_circles, Bounds bounds)
{
    // --- 1. UPDATE PHASE (Integrate Movement) ---
    // Move all circles based on their current velocities.
    for (int i = 0; i < num_circles; i++)
    {
        // P_new = P_old + V * dt (assuming circle_move handles the delta time)
        circle_move(circles[i]);
    }

    // --- 2. RESOLVE PHASE (Handle All Collisions) ---
    // A. Resolve Circle-Circle Collisions
    // This resolves overlaps and applies rebound velocities for pairs.
    collision_check_circles(circles);

    // B. Resolve Circle-Wall Collisions
    // This prevents spheres from getting stuck in the walls.
    for (int i = 0; i < num_circles; i++)
    {
        collision_resolve_bounds(circles[i], bounds);
    }
}
