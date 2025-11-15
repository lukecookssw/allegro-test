#ifndef SPATIAL_GRID_H
#define SPATIAL_GRID_H

#include "../circle/circle.h"

struct CircleNode;

typedef struct CircleNode
{
    Circle* circle;
    struct CircleNode* next;
} CircleNode;

typedef struct
{
    CircleNode* head;
    int count;
} CircleList;

typedef struct
{
    int rows;
    int columns;
    int cell_width;
    int cell_height;
    int world_width;
    int world_height;
    CircleList** cells;
} SpatialGrid;

SpatialGrid* grid_create(int world_w, int world_h, int cell_w, int cell_h);
void grid_clear(SpatialGrid* grid);
void grid_insert(SpatialGrid* grid, Circle* circle);
void grid_get_nearby_circles(SpatialGrid* grid, Circle* circle, CircleList* out);
void grid_destroy(SpatialGrid* grid);

#endif
