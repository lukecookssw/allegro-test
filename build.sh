#!/usr/bin/env bash
gcc -g -O0 -o main.out main.c \
window_settings.c \
lib/collision/circle_collider/circle_collider.c \
lib/collision/window_bounds_collider/window_bounds_collider.c \
lib/spatial_grid/spatial_grid.c \
lib/window/window.c \
lib/circle/circle.c \
$(pkg-config --cflags --libs allegro-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5) -lm
