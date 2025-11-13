#include "collision.h"
#include "../circle/circle.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Add collision flags to prevent repeated processing
static bool** collision_processed = NULL;
static int collision_object_count = 0;

// prototypes
void apply_rebound_velocities(Circle* c1, Circle* c2, float nx, float ny);


void set_collision_object_count(int count)
{
    // Free existing array if it exists
    collision_object_count = count;
    if (collision_processed != NULL)
    {
        for (int i = 0; i < collision_object_count; i++)
        {
            free(collision_processed[i]);
        }
        free(collision_processed);
    }

    // Allocate new 2D array
    collision_object_count = count;
    collision_processed = malloc(count * sizeof(bool*));
    for (int i = 0; i < count; i++)
    {
        collision_processed[i] = calloc(count, sizeof(bool));
    }
}

void collision_resolve_bounds(Circle* circle, Bounds bounds)
{
    float cx = circle->position[0];
    float cy = circle->position[1];
    float r = circle->radius;

    // Check Left/Right Bounds
    if (cx - r < 0.0f) // Colliding with Left Wall (x=0)
    {
        // 1. Position Resolution (Push out)
        circle->position[0] = r;

        // 2. Velocity Rebound (Reflect velocity based on normal vector (1, 0))
        circle->velocity[0] = -circle->velocity[0];
        // OR using math:
        // float normal_x = 1.0f;
        // circle->velocity[0] -= (1.0f + 1.0f) * (circle->velocity[0] * normal_x) * normal_x;
    }
    else if (cx + r > bounds.width) // Colliding with Right Wall
    {
        // 1. Position Resolution
        circle->position[0] = bounds.width - r;

        // 2. Velocity Rebound (Reflect velocity based on normal vector (-1, 0))
        circle->velocity[0] = -circle->velocity[0];
    }

    // Check Top/Bottom Bounds
    if (cy - r < 0.0f) // Colliding with Top Wall (y=0)
    {
        // 1. Position Resolution
        circle->position[1] = r;

        // 2. Velocity Rebound (Reflect velocity based on normal vector (0, 1))
        circle->velocity[1] = -circle->velocity[1];
    }
    else if (cy + r > bounds.height) // Colliding with Bottom Wall
    {
        // 1. Position Resolution
        circle->position[1] = bounds.height - r;

        // 2. Velocity Rebound (Reflect velocity based on normal vector (0, -1))
        circle->velocity[1] = -circle->velocity[1];
    }
}

void collision_check_circles(Circle** circle_array)
{
    for (int i = 0; i < collision_object_count; i++)
    {
        Circle* c1 = circle_array[i];
        for (int j = i + 1; j < collision_object_count; j++)
        {
            Circle* c2 = circle_array[j];
            if (c1->current_quadrant == c2->current_quadrant)
            {
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

                        // change the colour of the circles to a random colour
                        circle_change_colour(c1);
                        circle_change_colour(c2);
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
