#include "collision.h"
#include "../circle/circle.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Add collision flags to prevent repeated processing
static bool collision_processed[3][3] = {false};

// prototypes
void apply_rebound_velocities(Circle* c1, Circle* c2, float nx, float ny);

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

void collision_check_circles(Circle** circle_array)
{
    for (int i = 0; i < 3; i++)
    {
        Circle* c1 = circle_array[i];
        for (int j = i + 1; j < 3; j++)
        {
            Circle* c2 = circle_array[j];
            float dx = c2->position[0] - c1->position[0];
            float dy = c2->position[1] - c1->position[1];
            float distance = sqrtf(dx * dx + dy * dy);
            float radius_sum = c1->radius + c2->radius;
            
            if (distance < radius_sum)
            {
                // Collision detected!
                if (!collision_processed[i][j])
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
                    apply_rebound_velocities(c1, c2, nx, ny);

                    // Set flag to prevent processing this collision again this frame
                    collision_processed[i][j] = true;
                }
            }
            else
            {
                // Reset collision flag when circles are separated
                collision_processed[i][j] = false;
            }
        }
    }
}

// Physics calculation to update velocities in place
void apply_rebound_velocities(Circle* c1, Circle* c2, float nx, float ny)
{
    // Calculate relative velocity (c1_vel - c2_vel)
    float dvx = c1->velocity[0] - c2->velocity[0];
    float dvy = c1->velocity[1] - c2->velocity[1];
    
    // Calculate velocity along the normal (dot product)
    float velocity_along_normal = dvx * nx + dvy * ny;

    // Check if circles are already moving apart. 
    // If so, do nothing to prevent 'sticky' collisions.
    if (velocity_along_normal < 0.0f)
    {
        return; 
    }
    
    // Coefficient of Restitution (e): 1.0f for perfect elastic collision (no energy loss)
    float restitution = 1.0f; 
    
    // Calculate collision impulse (your formula is correct for equal, ignored masses)
    float impulse = -(1.0f + restitution) * velocity_along_normal / 2.0f;
    
    // Apply the impulse to update velocities
    c1->velocity[0] += impulse * nx;
    c1->velocity[1] += impulse * ny;
    c2->velocity[0] -= impulse * nx;
    c2->velocity[1] -= impulse * ny;
}