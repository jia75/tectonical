#include "map.h"
#include "hash.h"
#include "tectonics.h"

#ifndef TECTONICAL_GENERATOR_H
#define TECTONICAL_GENERATOR_H

#define LAND_RATE 0.30

#define TECTONIC_VOLATILITY 128

#define TECTONIC_IMPACT_MAX_RANGE 50

#define TECTONIC_IMPACT_DIMINISHING_FACTOR 0.8

#define LAND_PLATE_HEIGHT 50

#define TECTONIC_IMPACT_FACTOR 4

/* ======== generator.c ======== */

extern void diffuseMap(Map **mapPtr, float factor);

extern void generateTectonics(Map **mapPtr, int count, int seed);

extern void generateTectonicVectors(TectonicVector ***vecsPtr, int count,
        int seed);

extern void generateHeightmap(Map *tectonicMap, TectonicVector **tectonicVecs, 
        Map **heighMapPtr, int seed);

#endif /* TECTONICAL_GENERATOR_H */