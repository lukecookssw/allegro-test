#ifndef COLLIDER_H
#define COLLIDER_H

#include "../person/person.h"
#include <stdbool.h>

bool will_collide_with_border_x(struct Person *p, int width, int height);
bool will_collide_with_border_y(struct Person *p, int width, int height);


#endif