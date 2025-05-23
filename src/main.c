#include <stdlib.h>
#include <stdio.h>

#include "map.h"
#include "hash.h"

char density[10] =  {' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'};

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

void generateTectonics(Map **mapPtr, int count, int seed) {
    Map *map = *mapPtr;
    int rngCounter = seed;
    clearMap(map);
    for (int i = 0; i < count; ++i) {
        map->map[hashInRange(map->height, rngCounter)][hashInRange(map->height,
                rngCounter + 1)] = i+1;
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

int main() {
    Map *testMap;
    mallocMap(&testMap, 1000, 1000);

    generateTectonics(&testMap, 100, 75);

    // renderMap(testMap);
    // (9-1)/7 + 1

    char c;

    while (c != 'x') {
        while ((c = getc(stdin)) == '\n');
        // diffuseMap(&testMap, 0.5);
        renderMap(testMap);
        if (c == 'd') {
            showMapValues(testMap);
        }
        if (c == 'p') {
            renderToPpm(testMap, 15);
        }
    }

    freeMap(testMap);
    return 0;
}