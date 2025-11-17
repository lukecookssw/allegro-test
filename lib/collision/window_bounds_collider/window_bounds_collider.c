
#include "window_bounds_collider.h"


// private prototypes
bool wbcoll_will_collide_top(Circle* c);
bool wbcoll_will_collide_bottom(Circle* c, int window_height);
bool wbcoll_will_collide_left(Circle* c);
bool wbcoll_will_collide_right(Circle* c, int window_width);


void wbcoll_rebound_velocity(Circle* c, Window window)
{
    if(wbcoll_will_collide_top(c) || wbcoll_will_collide_bottom(c, window.height))
        c->velocity[1] = -c->velocity[1];

    if (wbcoll_will_collide_left(c) || wbcoll_will_collide_right(c, window.width))
        c->velocity[0] = -c->velocity[0];
}

bool wbcoll_will_collide_top(Circle* c)
{
    return (c->position[1] - c->radius) <= 0;
}

bool wbcoll_will_collide_bottom(Circle* c, int window_height)
{
    return (c->position[1] + c->radius) >= window_height;
}

bool wbcoll_will_collide_left(Circle* c)
{
    return (c->position[0] - c->radius) <= 0;
}

bool wbcoll_will_collide_right(Circle* c, int window_width)
{
    return (c->position[0] + c->radius) >= window_width;
}
