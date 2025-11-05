#include "collision.h"
#include "../circle/circle.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Add collision flags to prevent repeated processing
static bool collision_processed[3][3] = {false};

// prototypes
float* calculate_rebound_velocities(Circle* c1, Circle* c2);

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
                // Only process if we haven't already handled this collision
                if (!collision_processed[i][j])
                {
                    printf("New collision detected! Distance: %.2f, Required: %.2f\n", distance, radius_sum);
                    printf("Before - C1 vel: (%.2f, %.2f), C2 vel: (%.2f, %.2f)\n", 
                           c1->velocity[0], c1->velocity[1], c2->velocity[0], c2->velocity[1]);
                    
                    // Separate overlapping circles first
                    float overlap = radius_sum - distance;
                    float nx = dx / distance;
                    float ny = dy / distance;
                    
                    // Move circles apart completely
                    float separation = overlap * 0.5f + 1.0f; // Add extra separation
                    c1->position[0] -= separation * nx;
                    c1->position[1] -= separation * ny;
                    c2->position[0] += separation * nx;
                    c2->position[1] += separation * ny;
                    
                    // Apply collision response
                    float* new_velocities = calculate_rebound_velocities(c1, c2);
                    c1->velocity[0] = new_velocities[0];
                    c1->velocity[1] = new_velocities[1];
                    c2->velocity[0] = new_velocities[2];
                    c2->velocity[1] = new_velocities[3];
                    
                    printf("After - C1 vel: (%.2f, %.2f), C2 vel: (%.2f, %.2f)\n", 
                           c1->velocity[0], c1->velocity[1], c2->velocity[0], c2->velocity[1]);
                    
                    free(new_velocities);
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

float* calculate_rebound_velocities(Circle* c1, Circle* c2)
{
    // Calculate the normal vector
    float nx = c2->position[0] - c1->position[0];
    float ny = c2->position[1] - c1->position[1];
    float distance = sqrtf(nx * nx + ny * ny);
    
    if (distance == 0.0f)
    {
        nx = 1.0f;
        ny = 0.0f;
    }
    else
    {
        nx /= distance;
        ny /= distance;
    }
    
    // Calculate relative velocity
    float dvx = c1->velocity[0] - c2->velocity[0];
    float dvy = c1->velocity[1] - c2->velocity[1];
    
    // Calculate velocity along the normal
    float velocity_along_normal = dvx * nx + dvy * ny;
    
    printf("Normal: (%.2f, %.2f), Vel along normal: %.2f\n", nx, ny, velocity_along_normal);
    
    // Calculate collision response
    float restitution = 1.0f; // Perfect elastic collision
    float impulse = -(1 + restitution) * velocity_along_normal / 2.0f;
    
    printf("Impulse: %.2f\n", impulse);
    
    float* velocities = malloc(4 * sizeof(float));
    velocities[0] = c1->velocity[0] + impulse * nx;
    velocities[1] = c1->velocity[1] + impulse * ny;
    velocities[2] = c2->velocity[0] - impulse * nx;
    velocities[3] = c2->velocity[1] - impulse * ny;
    return velocities;
}