#include "collision.h"
#include "../circle/circle.h"

CollisionType collision_check_bounds(Circle* circle, Bounds bounds)
{
    float cx = circle->position[0];
    float cy = circle->position[1];
    float r = circle->radius;

    CollisionType collision = NO_COLLISION;
    bool collision_x = false;
    bool collision_y = false;
    if (cx - r < 0 || cx + r > bounds.width)
    {
        collision_x = true;
    }
    if (cy - r < 0 || cy + r > bounds.height)
    {
        collision_y = true;
    }
    if (collision_x && collision_y)
    {
        collision = BOTH_COLLISION;
    }
    else if (collision_y)
    {
        collision = VERTICAL_COLLISION;
    }
    else if (collision_x)
    {
        collision = HORIZONTAL_COLLISION;
    }

    return collision;
}