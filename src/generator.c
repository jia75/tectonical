#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "generator.h"

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

void generateTectonicVectors(TectonicVector ***vecsPtr, int count, int seed,
        Config *conf) {
    /* To avoid accidental alignments */
    int rngCounter = seed + randomHash(seed);
    *vecsPtr = malloc(count * sizeof(TectonicVector *));
    TectonicVector **vecs = *vecsPtr;
    for (int i = 0; i < count; ++i) {
        vecs[i] = malloc(sizeof(TectonicVector));
        vecs[i]->isLand = (hashInRange(1000, rngCounter) < (int)(conf->landRate
                *1000));
        vecs[i]->x = ((float)hashInRange(1000, rngCounter+1)-500)/1000;
        vecs[i]->y = ((float)hashInRange(1000, rngCounter+2)-500)/1000;
        rngCounter += 3;
    }
}

void generateTectonics(Map **mapPtr, int count, int seed, Config *conf) {
    Map *map = *mapPtr;
    int rngCounter = seed;
    clearMap(map);
    for (int i = 0; i < count; ++i) {
        if (map->map[hashInRange(map->height, rngCounter)]
                [hashInRange(map->width, rngCounter + 1)]) {
            ++rngCounter;
            continue;
        }
        map->map[hashInRange(map->height, rngCounter)][hashInRange(map->width,
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
                if (randomHash(rngCounter) < conf->tectonicVolatility) {
                    ++rngCounter;
                    continue;
                }
                ++rngCounter;
                int neighbors = ((i>0&&map->map[i-1][j]!=0)*8)|((j>0&&
                        map->map[i][j-1]!=0)*4)|((i+1<map->height&&
                        map->map[i+1][j]!=0)*2)|(j+1<map->width&&
                        map->map[i][j+1]!=0);
                int neighborCount = (i>0&&map->map[i-1][j]!=0)+(j>0&&
                        map->map[i][j-1]!=0)+(i+1<map->height&&
                        map->map[i+1][j]!=0)+(j+1<map->width&&
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

static void printVector(TectonicVector *vector) {
    fprintf(stdout, "ℹ Vector: (%f, %f)\n", vector->x, vector->y);
    return;
}

void generateHeightmap(Map *tectonicMap, TectonicVector **tectonicVecs, 
        Map **heightMapPtr, int seed, Config *conf) {
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
            map->map[i][j] = tectonicVecs[ownPlate]->isLand?conf->
                    landPlateHeight:conf->seaPlateHeight;
            /* fprintf(stdout, "ℹ Starting pixel %d %d; value is %f\n",
                    i, j, map->map[i][j]); */
            for (int direction = 0; direction < 4; ++direction) {
                /* fprintf(stdout, "ℹ Direction %d\n",
                    direction); */
                for (int k = 1; k < conf->tectonicImpactMaxRange; ++k) {
                    int targetI = i+((direction+1)%2)*(direction%3-1)*k;
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

                    float angle = atan2(calibratedTarget->y, 
                            calibratedTarget->x);
                    map->map[i][j] += conf->tectonicImpactFactor*(direction/2*
                            2-1)*sin(angle + ((direction)%2)*M_PI/2)/pow(k, 
                            conf->tectonicImpactDiminishingFactor);
                }
            }
        }
    }
    fprintf(stdout, "╚ Heightmap Generation: 100%%\n");

    free(calibratedTarget);
}

/* intensity -1 is basic for now */
void gaussianBlur(Map **mapPtr, float intensity, Config *conf) {
    Map *tempMap;

    Map *map = *mapPtr;

    mallocMap(&tempMap, map->height, map->width);

    clearMap(tempMap);

    for (int i = 0; i < map->height; ++i) {
        for (int j = 0; j < map->width; ++j) {
            float sum = map->map[i][j];
            float count = 1;
            for (int k = 1; k < conf->gaussianRange; ++k) {
                if (j+k < map->width) {
                    sum += map->map[i][j+k] * pow((float)k, intensity);
                    count += (pow((float)k, intensity));
                }
                if (j-k >= 0) {
                    sum += map->map[i][j-k] * pow((float)k, intensity);
                    count += (pow((float)k, intensity));
                }
            }
            tempMap->map[i][j] = sum/count;
        }
    }

    Map *swapMap;
    swapMap = tempMap;
    tempMap = map;
    map = swapMap;

    clearMap(tempMap);

    for (int i = 0; i < map->height; ++i) {
        for (int j = 0; j < map->width; ++j) {
            float sum = map->map[i][j];
            float count = 1;
            for (int k = 1; k < conf->gaussianRange; ++k) {
                if (i+k < map->height) {
                    sum += map->map[i+k][j] * pow((float)k, intensity);
                    count += (pow((float)k, intensity));
                }
                if (i-k >= 0) {
                    sum += map->map[i-k][j] * pow((float)k, intensity);
                    count += (pow((float)k, intensity));
                }
            }
            tempMap->map[i][j] = sum/count;
        }
    }

    freeMap(map);
    (*mapPtr) = tempMap;
    
    return;
}