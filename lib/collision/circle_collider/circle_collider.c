#include "circle_collider.h"
#include "../../spatial_grid/spatial_grid.h"

#include <math.h>

// private prototypes
void ccoll_process_cell(SpatialGrid *grid, CircleList *cell);
void ccoll_process_circle(Circle* c, CircleList* nearby);
void ccoll_calculate_circle_collision(Circle* c1, Circle* c2);
void ccoll_apply_rebound_velocities(Circle* c1, Circle* c2, float nx, float ny);

void ccoll_rebound_velocity(SpatialGrid *grid)
{
    for (int row_index = 0; row_index < grid->rows; row_index++)
    {
        for (int col_index = 0; col_index < grid->columns; col_index++)
        {
            CircleList *cell = &grid->cells[row_index][col_index];
            ccoll_process_cell(grid, cell);
        }
    }
}

void ccoll_process_cell(SpatialGrid *grid, CircleList *cell)
{
    if (cell->count == 0)
        return;

    
    CircleNode* current = cell->head;
    
    // get nearby circles from neighbouring cells once for this cell
    CircleList nearbyCircles;
    grid_get_nearby_circles(grid, current->circle, &nearbyCircles);

    // for each circle in the cell, we need to check whether they rebound against other nearby circles (including those in neighbouring cells)
    while(current != NULL)
    {
        Circle* c = current->circle;
        ccoll_process_circle(c, &nearbyCircles);
        current = current->next;
    }
    
    // No need to free - nodes are managed by the pool and will be reset on next grid_clear()
    
}

void ccoll_process_circle(Circle* c1, CircleList* nearby)
{
    if(nearby->count == 0)
        return;

    //printf("Circle %d checking %d nearby circles\n", c1->id, nearby->count);

    CircleNode* current = nearby->head;
    while(current != NULL)
    {
        Circle* c2 = current->circle;
        if(c1->id < c2->id) // prevent duplicate and self-collision
            ccoll_calculate_circle_collision(c1, c2);
        
        current = current->next;
    }
}

void ccoll_calculate_circle_collision(Circle* c1, Circle* c2)
{
    float dx = c2->position[0] - c1->position[0];
    float dy = c2->position[1] - c1->position[1];
    float distance = sqrtf(dx * dx + dy * dy);
    float radius_sum = c1->radius + c2->radius;

    if (distance < radius_sum)
    {
        // 1. Separation (Resolving Overlap)
        float overlap = radius_sum - distance;

        // Handle the case where the circles are perfectly stacked (distance == 0)
        if (distance == 0.0f)
        {
            // Choose an arbitrary normal to separate them
            dx = 1.0f;
            dy = 0.0f;
            distance = 1.0f;
        }

        // Calculate the unit normal vector (direction of collision)
        float nx = dx / distance;
        float ny = dy / distance;

        // Move circles apart. The total separation distance is 'overlap'.
        // We move each circle half of that distance.
        // Adding a small epsilon (0.001f) can prevent them from re-colliding immediately.
        float separation = overlap * 0.5f + 0.001f;

        c1->position[0] -= separation * nx;
        c1->position[1] -= separation * ny;
        c2->position[0] += separation * nx;
        c2->position[1] += separation * ny;

        // 2. Apply Collision Response (Velocity Rebound)
        ccoll_apply_rebound_velocities(c1, c2, nx, ny);

        // change the colour of the circles to a random colour
        circle_change_colour(c1);
        circle_change_colour(c2);
    }
}

void ccoll_apply_rebound_velocities(Circle* c1, Circle* c2, float nx, float ny)
{
    // Calculate relative velocity (c1_vel - c2_vel)
    float dvx = c1->velocity[0] - c2->velocity[0];
    float dvy = c1->velocity[1] - c2->velocity[1];

    // Calculate velocity along the normal (dot product)
    float velocity_along_normal = dvx * nx + dvy * ny;

    // Check if circles are already moving apart.
    // If so, do nothing to prevent 'sticky' collisions.
    if (velocity_along_normal < 0.0f)
        return;

    // Coefficient of Restitution (e): 1.0f for perfect elastic collision (no energy loss)
    float restitution = 1.0f;

    // Calculate collision impulse scalar
    float impulse = -(1.0f + restitution) * velocity_along_normal / 2.0f;

    // Apply the impulse to update velocities
    c1->velocity[0] += impulse * nx;
    c1->velocity[1] += impulse * ny;
    c2->velocity[0] -= impulse * nx;
    c2->velocity[1] -= impulse * ny;
}
