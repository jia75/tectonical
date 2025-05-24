#include <stdlib.h>
#include <stdio.h>

#include "map.h"
// #include "hash.h"
#include "tectonics.h"

int main() {
    fprintf(stdout, "Tectonic version 1.0.0, Copyright (C) 2025 D. Hargitt\n\
Tectonic comes with ABSOLUTELY NO WARRANTY; for details\n\
view LICENSE.md. This is free software, and you are\n\
welcome to redistribute it under certain conditions.\n");

    Map *testMap;
    mallocMap(&testMap, 1000, 1000);


    int tectonicCount = 100;
    generateTectonics(&testMap, 100, 75);

    int colorRange = (tectonicCount-1)/7 + 1;

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