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

static int max(int a, int b) {
    return a>b?a:b;
}

void renderToPpmBw(Map *map, int colorRange) {
    FILE *out = fopen("output-bw.ppm", "w");
    fprintf(out, "P3\n%d %d\n%d\n", map->width, map->height, colorRange);
    for (int i = 0; i < map->height; ++i) {
        for (int j = 0; j < map->width; ++j) {
            int val = (int)(map->map[i][j]);
            val = max(val, 0);
            fprintf(out, "%d %d %d\n", val, val, val);
        }
    }
    fclose(out);
}

void renderToRealistic(Map *map, int colorRange, int seaLevel) {
    FILE *out = fopen("output-real.ppm", "w");
    fprintf(out, "P3\n%d %d\n%d\n", map->width, map->height, colorRange);
    for (int i = 0; i < map->height; ++i) {
        for (int j = 0; j < map->width; ++j) {
            int val = (int)(map->map[i][j]);
            val = max(val, 0);
            if (val < seaLevel) {
                fprintf(out, "0 0 %d\n", val + colorRange/2);
            } else {
                fprintf(out, "%d %d %d\n", (val-seaLevel)*10, val + 
                        colorRange/2, (val-seaLevel)*10);
            }

        }
    }
    fclose(out);
}

void renderTectonicVectors(Map *map, TectonicVector **vecs) {
    FILE *out = fopen("output-vecs.ppm", "w");
    fprintf(out, "P3\n%d %d\n256\n", map->width, map->height);
    for (int i = 0; i < map->height; ++i) {
        for (int j = 0; j < map->width; ++j) {
            int plate = (int)(map->map[i][j]);
            fprintf(out, "%d %d %d\n", (int)(vecs[plate]->x*255),
                    (int)(vecs[plate]->y*255), vecs[plate]->isLand*255);
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

void renderToRealisticBands(Map *map, int seaLevel) {
    FILE *out = fopen("output-real-bands.ppm", "w");
    fprintf(out, "P3\n%d %d\n%d\n", map->width, map->height, 10);
    for (int i = 0; i < map->height; ++i) {
        for (int j = 0; j < map->width; ++j) {
            int val = (int)(map->map[i][j]);
            val = max(val, 0);
            if (val < seaLevel) {
                fprintf(out, "0 0 %d\n", (4 + 6*val/seaLevel)%11);
            } else if (val < seaLevel + 5) {
                fprintf(out, "9 9 0\n");
            } else if (val < seaLevel + 15) {
                fprintf(out, "0 9 0\n");
            } else if (val < seaLevel + 25) {
                fprintf(out, "9 9 9\n");
            } else {
                fprintf(out, "10 10 10\n");
            }

        }
    }
    fclose(out);
}