#include <stdlib.h>
#include <stdio.h>

#include "map.h"
#include "tectonics.h"

int main() {
    fprintf(stdout, "Tectonical version 1.0.0, Copyright (C) 2025 D. Hargitt\n\
Tectonical comes with ABSOLUTELY NO WARRANTY; for details view LICENSE.md. This\
\nis free software, and you are welcome to redistribute it under certain \n\
conditions.\n");

    fprintf(stdout, "ℹ Starting Generation\n");

    Map *testMap;
    mallocMap(&testMap, 1000, 1000);


    int tectonicCount = 50;
    generateTectonics(&testMap, tectonicCount, 75);

    fprintf(stdout, "✔ Tectonics Generated\n");

    Map *heightMap;
    mallocMap(&heightMap, 1000, 1000);

    TectonicVector **tectonicVecs;

    generateTectonicVectors(&tectonicVecs, tectonicCount, 75);

    fprintf(stdout, "✔ Vectors Generated\n");

    generateHeightmap(testMap, tectonicVecs, &heightMap, 75);

    fprintf(stdout, "✔ Heightmap Generated\n");

    int colorRange = (tectonicCount-1)/7 + 1;

    renderToPpmBw(heightMap, 100);
    renderToRealistic(heightMap, 100, 25);
    renderToPpm(testMap, colorRange);
    
    free(tectonicVecs); /* This doesn't recursively do its job */
    freeMap(testMap);
    freeMap(heightMap);
    return 0;

    char c;

    while (c != 'x') {
        while ((c = getc(stdin)) == '\n');
        // diffuseMap(&testMap, 0.5);
        renderMap(testMap);
        if (c == 'd') {
            showMapValues(testMap);
        }
        if (c == 'p') {
            renderToPpm(testMap, colorRange);
        }
    }

    freeMap(testMap);
    return 0;
}