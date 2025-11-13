
// #include "grid.h"
// #include "../circle/circle.h"
// #include <stdlib.h>

// // prototypes
// void init_grid(int width, int height, int circle_radius);
// void update_grid(Circle **circles);
// void draw_grid();
// void destroy_grid();

// // int* grid;
// int circle_count;
// int *top_left_circles;
// int *top_right_circles;
// int *bottom_left_circles;
// int *bottom_right_circles;

// int win_height;
// int win_width;

// void init_grid(int width, int height, int num_circles)
// {
//     // TODO: replace hard-coded grid
//     circle_count = num_circles;
//     win_height = height;
//     win_width = width;

//     top_left_circles = malloc(circle_count * sizeof(int));
//     top_right_circles = malloc(circle_count * sizeof(int));
//     bottom_left_circles = malloc(circle_count * sizeof(int));
//     bottom_right_circles = malloc(circle_count * sizeof(int));
// }

// void update_grid(Circle **circles)
// {
//     for (int i = 0; i < circle_count; i++)
//     {
//         // determine what quadrant the circle is in
//         int circle_pos_x = circles[i]->position[0];
//         int circle_pos_y = circles[i]->position[1];

//         bool left = false;
//         bool top = false;
//         if (circle_pos_x <= (win_width / 2))
//             left = true;
//         if (circle_pos_y <= (win_height / 2))
//             top = true;

//         // find and remove this id from all quadrants (slooow)
//         for (int i = 0; i < circle_count; i++)
//         {

//         }
//     }
// }

// void draw_grid()
// {
//     // implementation
// }

// void destroy_grid()
// {
//     // implementation
// }
