#include "map.h"
#include "hash.h"
#include "config.h"
#include "tectonics.h"

#ifndef TECTONICAL_GENERATOR_H
#define TECTONICAL_GENERATOR_H

/* ======== generator.c ======== */

extern void diffuseMap(Map **mapPtr, float factor);

extern void generateTectonics(Map **mapPtr, int count, int seed, Config *conf);

extern void generateTectonicVectors(TectonicVector ***vecsPtr, int count,
        int seed, Config *conf);

extern void generateHeightmap(Map *tectonicMap, TectonicVector **tectonicVecs, 
        Map **heighMapPtr, int seed, Config *conf);

extern void gaussianBlur(Map **mapPtr, float intensity, Config *conf);

#endif /* TECTONICAL_GENERATOR_H */