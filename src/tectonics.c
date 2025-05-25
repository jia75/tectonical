#include <stdlib.h>
#include <math.h>

#include <stdio.h>

#include "tectonics.h"

/* factor --> proportion of a cell's value that will get diffused 
0.8 < factor --> weird behavior */
void diffuseMap(Map **mapPtr, float factor) {
    Map *tempMap;

    Map *map = *mapPtr;

    mallocMap(&tempMap, map->height, map->width);

    clearMap(tempMap);

    for (int i = 0; i < map->height; ++i) {
        for (int j = 0; j < map->width; ++j) {
            tempMap->map[i][j] += map->map[i][j];
            if (i > 0) {
                /* diffuse up */
                tempMap->map[i-1][j] += map->map[i][j] * (factor/4);
                tempMap->map[i][j] -= map->map[i][j] * (factor/4);
            }
            if (j > 0) {
                /* diffuse left */
                tempMap->map[i][j-1] += map->map[i][j] * (factor/4);
                tempMap->map[i][j] -= map->map[i][j] * (factor/4);
            }
            if (i+1 < map->height) {
                /* diffuse down */
                tempMap->map[i+1][j] += map->map[i][j] * (factor/4);
                tempMap->map[i][j] -= map->map[i][j] * (factor/4);
            }
            if (j+1 < map->width) {
                /* diffuse right */
                tempMap->map[i][j+1] += map->map[i][j] * (factor/4);
                tempMap->map[i][j] -= map->map[i][j] * (factor/4);
            }
        }
    }

    freeMap(*mapPtr);
    (*mapPtr) = tempMap;
    
    return;
}

void generateTectonicVectors(TectonicVector ***vecsPtr, int count, int seed) {
    /* To avoid accidental alignments */
    int rngCounter = seed + randomHash(seed);
    *vecsPtr = malloc(count * sizeof(TectonicVector *));
    TectonicVector **vecs = *vecsPtr;
    for (int i = 0; i < count; ++i) {
        vecs[i] = malloc(sizeof(TectonicVector));
        vecs[i]->isLand = (hashInRange(1000, rngCounter) < (int)(LAND_RATE*
                1000));
        vecs[i]->x = ((float)hashInRange(1000, rngCounter+1))/1000;
        vecs[i]->y = ((float)hashInRange(1000, rngCounter+2))/1000;
        rngCounter += 3;
    }
}

void generateTectonics(Map **mapPtr, int count, int seed) {
    Map *map = *mapPtr;
    int rngCounter = seed;
    clearMap(map);
    for (int i = 0; i < count; ++i) {
        if (map->map[hashInRange(map->height, rngCounter)][hashInRange(map->height,
                rngCounter + 1)]) {
            ++rngCounter;
            continue;
        }
        map->map[hashInRange(map->height, rngCounter)][hashInRange(map->height,
                rngCounter + 1)] = i;
        rngCounter += 2;
    }

    Map *tempMap;
    mallocMap(&tempMap, map->height, map->width);
    clearMap(tempMap);

    int containsEmpty = 1;
    while (containsEmpty) {
        containsEmpty = 0;
        for (int i = 0; i < map->height; ++i) {
            for (int j = 0; j < map->width; ++j) {
                if (map->map[i][j] != 0) {
                    tempMap->map[i][j] = map->map[i][j];
                    continue;
                }
                if (randomHash(rngCounter) >= TECTONIC_VOLATILITY) {
                    ++rngCounter;
                    continue;
                }
                ++rngCounter;
                int neighbors = ((i>0&&map->map[i-1][j]!=0)*8)|((j>0&&
                        map->map[i][j-1]!=0)*4)|((i+1<map->height&&
                        map->map[i+1][j]!=0)*2)|(j+1<map->width&
                        map->map[i][j+1]!=0);
                int neighborCount = (i>0&&map->map[i-1][j]!=0)+(j>0&&
                        map->map[i][j-1]!=0)+(i+1<map->height&&
                        map->map[i+1][j]!=0)+(j+1<map->width&
                        map->map[i][j+1]!=0);
                if (neighborCount == 0) {
                    containsEmpty = 1;
                    continue;
                }
                int randomNeighborChoice = 0;
                while (!(neighbors&randomNeighborChoice)) {
                    randomNeighborChoice = 1<<hashInRange(4, rngCounter);
                    ++rngCounter;
                }
                if ((neighbors&randomNeighborChoice) == 1) {
                    tempMap->map[i][j] = map->map[i][j+1];
                }
                if ((neighbors&randomNeighborChoice) == 2) {
                    tempMap->map[i][j] = map->map[i+1][j];
                }
                if ((neighbors&randomNeighborChoice) == 4) {
                    tempMap->map[i][j] = map->map[i][j-1];
                }  
                if ((neighbors&randomNeighborChoice) == 8) {
                    tempMap->map[i][j] = map->map[i-1][j];
                }
            }
        }
        Map *switchMap = tempMap;
        tempMap = map;
        map = switchMap;
    }
    freeMap(tempMap);
    *mapPtr = map;
    return;
}

void generateHeightmap(Map *tectonicMap, TectonicVector **tectonicVecs, 
        Map **heightMapPtr, int seed) {
    Map *map = *heightMapPtr;
    clearMap(map);

    TectonicVector *calibratedTarget = malloc(sizeof(TectonicVector));

    fprintf(stdout, "╔ Heightmap Generation: 0%%\n");

    for (int i = 0; i < map->height; ++i) {
        if (i%(map->height/5) == 0 && i > 0) {
            fprintf(stdout, "╠ Heightmap Generation: %d%%\n", i/
                    (map->height/5)*20);
        }
        for (int j = 0; j < map->width; ++j) {
            int ownPlate = (int)tectonicMap->map[i][j];
            map->map[i][j] = tectonicVecs[ownPlate]->isLand*LAND_PLATE_HEIGHT;
            /* fprintf(stdout, "ℹ Starting pixel %d %d; value is %f\n",
                    i, j, map->map[i][j]); */
            for (int direction = 0; direction < 4; ++direction) {
                /* fprintf(stdout, "ℹ Direction %d\n",
                    direction); */
                for (int k = 1; k < TECTONIC_IMPACT_MAX_RANGE; ++k) {
                    int targetI = i+((direction+1)%2)*(-1)*(direction%3-1)*k;
                    int targetJ = j+((direction)%2)*(-1)*((direction-1)%3-1)*k;
                    if (targetI < 0 || targetI >= map->height || targetJ < 0 ||
                            targetJ >= map->width) {
                        break;
                    }
                    int targetPlate = (int)tectonicMap->map[targetI][targetJ];
                    if (targetPlate == ownPlate) {
                        continue;
                    }
                    /* fprintf(stdout, "ℹ Target Plate %d\n", targetPlate); */
                    calibratedTarget->x = tectonicVecs[targetPlate]->x - 
                            tectonicVecs[ownPlate]->x;
                    calibratedTarget->y = tectonicVecs[targetPlate]->y - 
                            tectonicVecs[ownPlate]->y;
                    /* fprintf(stdout, "╠ Changing; gradient is %f\n",
                            calibratedTarget->y/calibratedTarget->x); */
                    float angle = atan(calibratedTarget->y/
                            calibratedTarget->x);
                    map->map[i][j] += TECTONIC_IMPACT_FACTOR*(direction/2-1)*
                            sin(angle + (direction%2)*M_PI/2)/pow(k, 
                            TECTONIC_IMPACT_DIMINISHING_FACTOR);
                }
            }
        }
    }
    fprintf(stdout, "╚ Heightmap Generation: 100%%\n");

    free(calibratedTarget);
}