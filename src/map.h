#include "tectonics.h"

#ifndef TECTONICAL_MAP_H
#define TECTONICAL_MAP_H

typedef struct {
    float **map;
    int height;
    int width;
} Map;

/* ======== map.c ======== */

extern void freeMap(Map *map);

extern void mallocMap(Map **mapPtr, int height, int width);

extern void clearMap(Map *map);

/* ======== map-render.c ======== */

extern void renderMap(Map *map);

extern void renderToPpm(Map *map, int colorRange);

extern void showMapValues(Map *map);

extern void renderToPpmBw(Map *map, int colorRange);

extern void renderToRealistic(Map *map, int colorRange, int seaLevel);

extern void renderTectonicVectors(Map *map, TectonicVector **vecs);

#endif /* TECTONIC_MAP_H */