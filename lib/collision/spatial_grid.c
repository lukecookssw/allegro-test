#include "spatial_grid.h"
#include <stdlib.h>
#include <unistd.h>

// prototypes
int get_circle_row(SpatialGrid *grid, Circle *circle);
int get_circle_column(SpatialGrid *grid, Circle *circle);

SpatialGrid *grid_create(int world_w, int world_h, int cell_w, int cell_h)
{
    SpatialGrid *grid = malloc(sizeof(SpatialGrid));

    grid->cell_height = cell_h;
    grid->cell_width = cell_w;

    grid->world_height = world_h;
    grid->world_width = world_w;

    grid->rows = world_h / cell_h;
    grid->columns = world_w / cell_w;

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
    for (int i = 0; i < grid->rows; i++)
    {
        for (int j = 0; j < grid->columns; j++)
        {
            CircleNode *current = grid->cells[i][j].head;
            while (current != NULL)
            {
                CircleNode *next = current->next;
                free(current);
                current = next;
            }

            grid->cells[i][j].head = NULL;
            grid->cells[i][j].count = 0;
        }
    }
}
void grid_insert(SpatialGrid *grid, Circle *circle)
{
    int row = get_circle_row(grid, circle);
    int col = get_circle_column(grid, circle);

    CircleNode *new_node = malloc(sizeof(CircleNode));
    new_node->circle = circle;
    new_node->next = NULL;

    if (grid->cells[row][col].head == NULL)
    {
        grid->cells[row][col].head = new_node;
        grid->cells[row][col].count = 1;
        return;
    }
    CircleNode *current = grid->cells[row][col].head;
    while (current->next != NULL)
        current = current->next;

    current->next = new_node;
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
                    CircleNode* new_node = malloc(sizeof(CircleNode));
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
