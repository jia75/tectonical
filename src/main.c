#include <stdlib.h>
#include <stdio.h>

#include "map.h"
#include "generator.h"

int main() {
    fprintf(stdout, "Tectonical version 1.0.0, Copyright (C) 2025 D. Hargitt\n\
Tectonical comes with ABSOLUTELY NO WARRANTY; for details view LICENSE.md. This\
\nis free software, and you are welcome to redistribute it under certain \n\
conditions.\n");

    fprintf(stdout, "❯ Starting Generation\n");

    Map *testMap;
    int xSize = 1000, ySize = 1000;
    mallocMap(&testMap, xSize, ySize);

    int seed = 69;

    int tectonicCount = 50;
    generateTectonics(&testMap, tectonicCount, seed);

    fprintf(stdout, "✔ Tectonics Generated\n");

    Map *heightMap;
    mallocMap(&heightMap, xSize, ySize);

    TectonicVector **tectonicVecs;

    generateTectonicVectors(&tectonicVecs, tectonicCount, seed);

    fprintf(stdout, "✔ Vectors Generated\n");

    generateHeightmap(testMap, tectonicVecs, &heightMap, seed);

    fprintf(stdout, "✔ Heightmap Generated\n");

    int colorRange = (tectonicCount-1)/7 + 1;

    renderToPpmBw(heightMap, 100);
    renderToRealistic(heightMap, 100, 45);
    renderToPpm(testMap, colorRange);
    renderTectonicVectors(testMap, tectonicVecs);

    free(tectonicVecs); /* Note: this doesn't recursively do its job */
    freeMap(testMap);
    freeMap(heightMap);

    return 0;
}