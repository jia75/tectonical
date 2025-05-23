#include <stdlib.h>

#include "map.h"

void freeMap(Map *map) {
    for (int i = 0; i< map->height; ++i) {
        free(map->map[i]);
    }
    free(map->map);
    free(map);
    return;
}

void mallocMap(Map **mapPtr, int height, int width) {
    *mapPtr = malloc(sizeof(Map));
    (*mapPtr)->height = height;
    (*mapPtr)->width = width;
    (*mapPtr)->map = malloc(height * sizeof(int*));
    for (int i = 0; i < height; ++i) {
        (*mapPtr)->map[i] = malloc(width * sizeof(int));
    }
    return;
}

void clearMap(Map *map) {
    for (int i = 0; i < map->height; ++i) {
        for (int j = 0; j < map->width; ++j) {
            map->map[i][j] = 0;
        }
    }
}