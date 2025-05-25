#include "map.h"
#include "hash.h"

#ifndef TECTONICAL_TECTONICS_H
#define TECTONICAL_TECTONICS_H

typedef struct {
    float x;
    float y;
    int isLand;
} TectonicVector;

#define LAND_RATE 0.30

#define TECTONIC_VOLATILITY 128

#define TECTONIC_IMPACT_MAX_RANGE 50

#define TECTONIC_IMPACT_DIMINISHING_FACTOR 1.4

#define LAND_PLATE_HEIGHT 30

#define TECTONIC_IMPACT_FACTOR 15

/* ======== tectonics.c ======== */

extern void diffuseMap(Map **mapPtr, float factor);

extern void generateTectonics(Map **mapPtr, int count, int seed);

extern void generateTectonicVectors(TectonicVector ***vecsPtr, int count,
        int seed);

extern void generateHeightmap(Map *tectonicMap, TectonicVector **tectonicVecs, 
        Map **heighMapPtr, int seed);

#endif /* TECTONIC_TECTONICS_H */