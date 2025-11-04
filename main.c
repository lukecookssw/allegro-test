// standard headers
#include <stdio.h>

// allegro5 headers
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

// custom headers
#include "window_settings.c"
#include "lib/circle/circle.h"
#include "lib/collision/collision.h"


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

    // TODO: Package font with application
    ALLEGRO_FONT *font = al_load_ttf_font("/usr/share/fonts/fira-code/FiraCode-Regular.ttf", 24, 0);
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

    Circle *c_1 = circle_create(50.0, al_map_rgb(255, 0, 0));
    // calculate random start position between 0 and window width/height
    float start_x = rand() % (640 - 100) + 50;
    float start_y = rand() % (480 - 100) + 50;
    circle_place(c_1, start_x, start_y);

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

        CollisionType collision = collision_check_bounds(c_1, (Bounds){WINDOW_WIDTH, WINDOW_HEIGHT});
        if (collision == HORIZONTAL_COLLISION)
        {
            circle_rebound(c_1, true, false);
        }
        else if (collision == VERTICAL_COLLISION)
        {
            circle_rebound(c_1, false, true);
        }
        else if (collision == BOTH_COLLISION)
        {
            circle_rebound(c_1, true, true);
        }
        circle_move(c_1);

        if (redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            circle_draw(c_1, true);
            
            
            al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello world!");
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
