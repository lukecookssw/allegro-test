#!/usr/bin/env bash
gcc -g -O0 -o main.out main.c window_settings.c lib/collision/spatial_grid.c lib/collision/collision.c lib/circle/circle.c $(pkg-config --cflags --libs allegro-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5) -lm
