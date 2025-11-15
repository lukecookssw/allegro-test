#include "spatial_grid.h"
#include <stdlib.h>

SpatialGrid* grid_create(int world_w, int world_h, int cell_w, int cell_h)
{
    SpatialGrid* grid   = malloc(sizeof(SpatialGrid));

    grid->cell_height   = cell_h;
    grid->cell_width    = cell_w;

    grid->world_height  = world_h;
    grid->world_width   = world_w;

    return grid;
}

void grid_clear(SpatialGrid* grid);
void grid_insert(SpatialGrid* grid, Circle* circle);
void grid_get_nearby_circles(SpatialGrid* grid, Circle* circle, CircleList* out);

void grid_destroy(SpatialGrid* grid)
{
    free(grid);
}
