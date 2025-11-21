#include "spatial_grid.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// prototypes
int get_circle_row(SpatialGrid *grid, Circle *circle);
int get_circle_column(SpatialGrid *grid, Circle *circle);
NodePool* node_pool_create(int capacity);
void node_pool_reset(NodePool* pool);
CircleNode* node_pool_allocate(NodePool* pool);
void node_pool_destroy(NodePool* pool);

SpatialGrid *grid_create(int circle_count, int world_w, int world_h, int cell_w, int cell_h)
{
    SpatialGrid *grid = malloc(sizeof(SpatialGrid));

    grid->circle_count = circle_count;

    grid->cell_height = cell_h;
    grid->cell_width = cell_w;

    grid->world_height = world_h;
    grid->world_width = world_w;

    grid->rows = world_h / cell_h;
    grid->columns = world_w / cell_w;

    // Create node pool - allocate enough for all circles plus neighbors
    // Each circle needs ~9 nodes (itself in 9 neighboring cells worst case)
    // Plus nearby list allocations (3x3 grid * avg circles per cell)
    int pool_capacity = circle_count * 20;
    grid->node_pool = node_pool_create(pool_capacity);

    // create an array of pointers
    grid->cells = malloc(grid->rows * sizeof(CircleList *));

    // grid->cells currently looks like
    // [ row1_ptr, row2_ptr, ... ]
    // these are pointers to arrays that we haven't initialised yet.

    // loop through each of the row pointers above
    for (int i = 0; i < grid->rows; i++)
    {
        // for every pointer, we malloc an actual array of cells - a CircleList for every column in the row
        grid->cells[i] = malloc(grid->columns * sizeof(CircleList));

        // so now, we know that:
        // grid->cells[0] is a pointer to the array of columns in the first row
        // grid->cells[0] = [col1, col2, ...]

        for (int j = 0; j < grid->columns; j++)
        {
            grid->cells[i][j].head = NULL;
            grid->cells[i][j].count = 0;
        }
    }

    return grid;
}

void grid_clear(SpatialGrid *grid)
{
    // Reset node pool instead of freeing individual nodes
    node_pool_reset(grid->node_pool);
    
    // Clear all cell lists
    for (int i = 0; i < grid->rows; i++)
    {
        for (int j = 0; j < grid->columns; j++)
        {
            grid->cells[i][j].head = NULL;
            grid->cells[i][j].count = 0;
        }
    }
}
void grid_insert(SpatialGrid *grid, Circle *circle)
{
    int row = get_circle_row(grid, circle);
    int col = get_circle_column(grid, circle);

    CircleNode *new_node = node_pool_allocate(grid->node_pool);
    new_node->circle = circle;
    new_node->next = grid->cells[row][col].head;
    
    grid->cells[row][col].head = new_node;
    grid->cells[row][col].count++;
}

void grid_get_nearby_circles(SpatialGrid *grid, Circle *circle, CircleList *out)
{
    out->head = NULL;
    out->count = 0;

    int circle_row_index = get_circle_row(grid, circle);
    int circle_col_index = get_circle_column(grid, circle);

    // one behind, one in front
    for (int row_offset = -1; row_offset <= 1; row_offset++)
    {
        // one above, one below
        for (int col_offset = -1; col_offset <= 1; col_offset++)
        {
            // row/col indexes to check
            int check_row = circle_row_index + row_offset;
            int check_col = circle_col_index + col_offset;

            if (check_row >= 0 && check_row < grid->rows &&
                check_col >= 0 && check_col < grid->columns)
            {
                if(grid->cells[check_row][check_col].count == 0)
                    continue;

                CircleNode* current = grid->cells[check_row][check_col].head;
                while(current != NULL)
                {
                    CircleNode* new_node = node_pool_allocate(grid->node_pool);
                    new_node->circle = current->circle;
                    new_node->next = out->head;
                    out->head = new_node;
                    out->count++;

                    current = current->next;
                }
            }
        }
    }
}

int get_circle_row(SpatialGrid *grid, Circle *circle)
{
    int row = (int)(circle->position[1] / grid->cell_height);

    // Clamp to valid bounds
    if (row < 0)
        return 0;
    if (row >= grid->rows)
        return grid->rows - 1;

    return row;
}

int get_circle_column(SpatialGrid *grid, Circle *circle)
{
    int col = (int)(circle->position[0] / grid->cell_width);

    // Clamp to valid bounds
    if (col < 0)
        return 0;
    if (col >= grid->columns)
        return grid->columns - 1;

    return col;
}

void grid_destroy(SpatialGrid *grid)
{
    if (grid)
    {
        // Free node pool
        node_pool_destroy(grid->node_pool);
        
        // Free each row
        for (int i = 0; i < grid->rows; i++)
        {
            free(grid->cells[i]);
        }

        // Free the array of row pointers
        free(grid->cells);

        // Free the grid itself
        free(grid);
    }
}

// Node Pool Implementation
NodePool* node_pool_create(int capacity)
{
    NodePool* pool = malloc(sizeof(NodePool));
    pool->nodes = malloc(capacity * sizeof(CircleNode));
    pool->capacity = capacity;
    pool->next_index = 0;
    return pool;
}

void node_pool_reset(NodePool* pool)
{
    pool->next_index = 0;
}

CircleNode* node_pool_allocate(NodePool* pool)
{
    if (pool->next_index >= pool->capacity)
    {
        // Pool exhausted - reallocate with double capacity
        int new_capacity = pool->capacity * 2;
        CircleNode* new_nodes = realloc(pool->nodes, new_capacity * sizeof(CircleNode));
        if (new_nodes == NULL)
        {
            // Realloc failed - fatal error
            fprintf(stderr, "Node pool exhausted and realloc failed!\n");
            exit(1);
        }
        pool->nodes = new_nodes;
        pool->capacity = new_capacity;
    }
    
    return &pool->nodes[pool->next_index++];
}

void node_pool_destroy(NodePool* pool)
{
    if (pool)
    {
        free(pool->nodes);
        free(pool);
    }
}
