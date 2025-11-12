
#ifndef GRID_H
#define GRID_H

#include "../circle/circle.h"

void init_grid(int width, int height, int circle_radius);
void update_grid(Circle** circles, int num_circles);
void draw_grid();
void destroy_grid();

#endif
