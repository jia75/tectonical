#include <stdlib.h>
#include <stdio.h>

typedef struct {
    float **map;
    int height;
    int width;
} Map;

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

void renderToPpm(Map *map) {
    FILE *out = fopen("output.ppm", "w");
    fprintf(out, "P3\n%d %d\n10\n", map->height, map->width);
    for (int i = 0; i < map->height; ++i) {
        for (int j = 0; j < map->width; ++j) {
            int val = (int)(map->map[i][j] > 9 ? 9 : map->map[i][j]);
            fprintf(out, "%d %d %d\n", val, val, val);
        }
    }
    fclose(out);
}

void clearMap(Map *map) {
    for (int i = 0; i < map->height; ++i) {
        for (int j = 0; j < map->width; ++j) {
            map->map[i][j] = 0;
        }
    }
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

/* Returns an integer from 0 to 255 */
int randomHash(int in) {
    return ((((in*in)^(in+150))*13-2)^in)%256;
}

/* Hash from 0 to max */
int hashInRange(int max, int in) {
    int maxt = max;
    int out = randomHash(in);
    while (maxt != 0) {
        out *= 256;
        out += randomHash(out^in);
        maxt /= 256;
    }
    return out%max;
}

void generateTectonics(Map **mapPrt, int count, int seed) {
    Map *map = *mapPrt;
    int rngCounter = seed;
    clearMap(*mapPrt);
    for (int i = 0; i < count; ++i) {
        map->map[hashInRange(map->height, rngCounter)][hashInRange(map->height,
                rngCounter + 1)] = 100;
        rngCounter += 2;
    }
    return;
}

int main() {
    Map *testMap;
    mallocMap(&testMap, 20, 20);

    generateTectonics(&testMap, 5, 73);

    renderMap(testMap);

    char c;

    while (c != 'x') {
        while ((c = getc(stdin)) == '\n');
        diffuseMap(&testMap, 0.5);
        renderMap(testMap);
        if (c == 'd') {
            showMapValues(testMap);
        }
        if (c == 'p') {
            renderToPpm(testMap);
        }
    }

    freeMap(testMap);
    return 0;
}