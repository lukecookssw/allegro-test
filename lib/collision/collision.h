#ifndef COLLISION_H
#define COLLISION_H

#include "../circle/circle.h"

typedef struct {
    int width;
    int height;
} Bounds;

void set_collision_object_count(int count);
void collision_resolve_bounds(Circle* circle, Bounds bounds);
void collision_check_circles(Circle** circle_array);

#endif