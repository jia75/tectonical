#include <stdio.h>

#include "map.h"

static char density[10] =  {' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'};

void renderMap(Map *map) {
    for (int i = 0; i < map->height; ++i) {
        for (int j = 0; j < map->width; ++j) {
            fprintf(stdout, "%c ", density[(int)(map->map[i][j] > 9 ? 9 :
                    map->map[i][j])]);
        }
        fprintf(stdout, "\n");
    }
}

void renderToPpm(Map *map, int colorRange) {
    FILE *out = fopen("output.ppm", "w");
    fprintf(out, "P3\n%d %d\n%d\n", map->width, map->height, colorRange);
    for (int i = 0; i < map->height; ++i) {
        for (int j = 0; j < map->width; ++j) {
            int val = (int)(map->map[i][j]);
            val += (val - 1)/7;
            fprintf(out, "%d %d %d\n", ((val%2)*(val+6)/7)%colorRange, 
                    ((val/2%2)*(val+6)/7)%colorRange, ((val/4%2)*(val+6)/7)%
                    colorRange);
        }
    }
    fclose(out);
}

void showMapValues(Map *map) {
    for (int i = 0; i < map->height; ++i) {
        for (int j = 0; j < map->width; ++j) {
            fprintf(stdout, "%02d ", (int)(map->map[i][j]));
        }
        fprintf(stdout, "\n");
    }
}