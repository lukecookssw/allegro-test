#include "person.h"
#include <stdlib.h>

struct Person* person_create(float pos_x, float pos_y, float size)
{
    struct Person *p = malloc(sizeof(struct Person));
    p->pos_x = pos_x;
    p->pos_y = pos_y;
    p->size = size;
    p->vec_direction[0] = 3;    // x direction
    p->vec_direction[1] = 2;    // y direction
    return p;
}

void person_move(struct Person *p)
{
    p->pos_x += p->vec_direction[0];
    p->pos_y += p->vec_direction[1];
}