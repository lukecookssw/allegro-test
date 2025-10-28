#include "collider.h"
#include "../person/person.h"
#include <stdbool.h>
#include <math.h>

bool will_collide(float pos, int max);

bool will_collide_with_border_x(struct Person *p, int width, int height)
{
    float size = p->vec_direction[0] > 0 
        ? p->size : -p->size;
    float p_next = p->pos_x + p->vec_direction[0] + size;
    return will_collide(p_next, width);
}

bool will_collide_with_border_y(struct Person *p, int width, int height)
{
    float size = p->vec_direction[1] > 0 
        ? p->size : -p->size;
    float p_next = p->pos_y + p->vec_direction[1] + size;
    return will_collide(p_next, height);
}

bool will_collide(float pos, int max)
{
    int i = pos;
    if (i > max || i <= 0)
        return true;
    return false;
}
