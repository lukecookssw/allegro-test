#ifndef COLLISION_H
#define COLLISION_H

#include "../circle/circle.h"

typedef enum {
    NO_COLLISION = 0,
    HORIZONTAL_COLLISION = 1,
    VERTICAL_COLLISION = 2,
    BOTH_COLLISION = 3
} CollisionType;

typedef struct {
    int width;
    int height;
} Bounds;

CollisionType collision_check_bounds(Circle* circle, Bounds bounds);

#endif