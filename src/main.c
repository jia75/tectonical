#include <stdlib.h>
#include <stdio.h>

#include "map.h"
#include "generator.h"

int main() {
    fprintf(stdout, "Tectonical version 1.0.0, Copyright (C) 2025 D. Hargitt\n\
Tectonical comes with ABSOLUTELY NO WARRANTY; for details view LICENSE.md. This\
\nis free software, and you are welcome to redistribute it under certain \n\
conditions.\n");

    fprintf(stdout, "ℹ Starting Generation\n");

    Map *testMap;
    mallocMap(&testMap, 1000, 1000);

    int seed = 75;

    int tectonicCount = 50;
    generateTectonics(&testMap, tectonicCount, seed);

    fprintf(stdout, "✔ Tectonics Generated\n");

    Map *heightMap;
    mallocMap(&heightMap, 1000, 1000);

    TectonicVector **tectonicVecs;

    generateTectonicVectors(&tectonicVecs, tectonicCount, seed);

    fprintf(stdout, "✔ Vectors Generated\n");

    /* 552 412 
    550 402 */

    fprintf(stdout, "ℹ Notice: Info on (552, 412). v = (%f, %f); p = %d\n",
            tectonicVecs[(int)testMap->map[412][552]]->x, 
            tectonicVecs[(int)testMap->map[412][552]]->y,
            (int)testMap->map[412][552]);

    fprintf(stdout, "ℹ Notice: Info on (550, 402). v = (%f, %f); p = %d\n",
            tectonicVecs[(int)testMap->map[402][550]]->x, 
            tectonicVecs[(int)testMap->map[402][550]]->y,
            (int)testMap->map[402][550]);

    generateHeightmap(testMap, tectonicVecs, &heightMap, seed);

    fprintf(stdout, "✔ Heightmap Generated\n");

    int colorRange = (tectonicCount-1)/7 + 1;

    renderToPpmBw(heightMap, 100);
    renderToRealistic(heightMap, 100, 25);
    renderToPpm(testMap, colorRange);
    renderTectonicVectors(testMap, tectonicVecs);
    
    int a = 0, b = 0;
    while (1) {
        fscanf(stdin, "%d %d", &a, &b);
        fprintf(stdout, "ℹ Notice: Info on (%d, %d). v = (%f, %f); p = %d\n",
            a, b, tectonicVecs[(int)testMap->map[a][b]]->x, 
            tectonicVecs[(int)testMap->map[a][b]]->y, (int)testMap->map[a][b]);
    }

    free(tectonicVecs); /* Note: this doesn't recursively do its job */
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