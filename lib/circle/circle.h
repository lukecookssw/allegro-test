#ifndef CIRCLE_H
#define CIRCLE_H

#include <allegro5/color.h>

typedef struct
{

    int id;
    ALLEGRO_COLOR colour;
    float radius;
    float position[2]; // position[0] = x, position[1] = y
    float velocity[2]; // velocity[0] = x, velocity[1] = y
} Circle;

Circle* circle_create(int id, float min_radius, float max_radius);
void circle_place(Circle* c, float position_x, float position_y, int max_speed);
void circle_move(Circle* c);
void circle_draw(Circle* c, bool filled);
void circle_change_colour(Circle* c);

#endif
