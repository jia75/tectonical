#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "map.h"
#include "generator.h"
#include "config.h"

int main(int argc, char **argv) {
    /* fprintf(stdout, "Tectonical version 1.0.0, Copyright (C) 2025 D. Hargitt\n\
Tectonical comes with ABSOLUTELY NO WARRANTY; for details view LICENSE.md. \
This \nis free software, and you are welcome to redistribute it under certain \
\nconditions.\n"); */

    if (argc < 2) {
        fprintf(stderr, "⚠ No arguments passed. Use the command help for help\
        \n");
        return 0;
    }

    if (!strcmp(argv[1], "config")) {
        fprintf(stderr, "⚠ Fill out later\n");
        return 0;
    }

    if (!strcmp(argv[1], "help")) {
        fprintf(stdout, "Tectonical version 1.0.0, Copyright (C) 2025 D. Hargitt\n\
Tectonical comes with ABSOLUTELY NO WARRANTY; for details view LICENSE.md. \
This \nis free software, and you are welcome to redistribute it under certain \
\nconditions.\n\n");
        fprintf(stdout, "Use generate to generate a map based on the settings \
located at \n`.tectonical.config`\n\nIf this file is not present, it will be \
generated with some default values\n");
        return 0;
    }

    if (strcmp(argv[1], "generate")) {
        fprintf(stderr, "⚠ Unknown command\n");
        return 0;
    }

    checkConfig();

    Config *conf;

    setupConfig(&conf);

    fprintf(stdout, "▶ Starting Generation\n");

    Map *testMap;
    int xSize = conf->width, ySize = conf->height;
    mallocMap(&testMap, xSize, ySize);

    int seed = conf->seed;

    int tectonicCount = conf->tectonicCount;
    generateTectonics(&testMap, tectonicCount, seed, conf);

    fprintf(stdout, "✔ Tectonics Generated\n");

    Map *heightMap;
    mallocMap(&heightMap, xSize, ySize);

    TectonicVector **tectonicVecs;

    generateTectonicVectors(&tectonicVecs, tectonicCount, seed, conf);

    fprintf(stdout, "✔ Vectors Generated\n");

    generateHeightmap(testMap, tectonicVecs, &heightMap, seed, conf);

    fprintf(stdout, "✔ Heightmap Generated\n");

    gaussianBlur(&heightMap, conf->gaussianDiminishingFactor, conf);

    fprintf(stdout, "✔ Blur Completed\n");

    int colorRange = (tectonicCount-1)/7 + 1;

    renderToPpmBw(heightMap, 100);
    renderToRealistic(heightMap, 100, conf->seaLevel);
    renderToRealisticBands(heightMap, conf->seaLevel);
    renderToPpm(testMap, colorRange);

    for (int i = 0; i < tectonicCount; ++i) {
        free(tectonicVecs[i]);
    }
    free(tectonicVecs);
    
    freeMap(testMap);
    freeMap(heightMap);

    fprintf(stdout, "✔ ALL Completed\n");

    return 0;
}