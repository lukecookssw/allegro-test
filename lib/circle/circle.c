#include "circle.h"
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>

Circle *circle_create(int id, float radius)
{
    Circle *c = malloc(sizeof(Circle));
    c->radius = radius;
    circle_change_colour(c);
    return c;
}

void set_quadrant(Circle* c)
{
    // determine quadrant (hard-code dimensions right now)
    bool isLeft = (int)c->position[0] <= (640 / 2);
    bool isTop = (int)c->position[1] <= (480 / 2);
    if (isLeft)
    {
        if (isTop)
            c->current_quadrant = TOP_LEFT;
        else
            c->current_quadrant = BOTTOM_LEFT;
    }
    else
    {
        if (isTop)
            c->current_quadrant = TOP_RIGHT;
        else
            c->current_quadrant = BOTTOM_RIGHT;
    }
}

void circle_place(Circle *c, float position_x, float position_y)
{
    c->position[0] = position_x;
    c->position[1] = position_y;

    set_quadrant(c);

    // set initial velocity, random direction and speed between 2 and 5
    float velocity_x = (rand() % 4) + 2;
    float velocity_y = (rand() % 4) + 2;
    c->velocity[0] = velocity_x;
    c->velocity[1] = velocity_y;
}
void circle_move(Circle *c)
{
    c->position[0] += c->velocity[0];
    c->position[1] += c->velocity[1];
    set_quadrant(c);
}

void circle_draw(Circle *c, bool filled)
{
    if (filled)
    {
        al_draw_filled_circle(c->position[0], c->position[1], c->radius, c->colour);
    }
    else
    {
        al_draw_circle(c->position[0], c->position[1], c->radius, c->colour, 2);
    }
}

void circle_rebound(Circle *c, bool rebound_x, bool rebound_y)
{
    if (rebound_x)
    {
        c->velocity[0] = -c->velocity[0];
    }
    if (rebound_y)
    {
        c->velocity[1] = -c->velocity[1];
    }
}

void circle_change_colour(Circle *c)
{
    ALLEGRO_COLOR colour = al_map_rgba(rand() % 256, rand() % 256, rand() % 256, 0.5);
    c->colour = colour;
}
