#include <stdio.h>
#include <stdlib.h>

#include "config.h"

void checkConfig() {
    FILE *fptr;
    if (!(fptr = fopen(".tectonical.config", "r"))) {
        fptr = fopen(".tectonical.config", "w");
        fprintf(
            fptr, 
            "seed=100\nwidth=256\nheight=256\nland-rate=0.3\ntectonic-volatili\
ty=128\ntectonic-impact-max-range=50\ntectonic-impact-diminishing-factor=0.4\n\
sea-plate-height=10\nland-plate-height=20\ntectonic-impact-factor=1.5\nsea-lev\
el=20\ngaussian-range=85\ngaussian-diminishing-factor=-2\ntectonic-count=11\n"
        );
    }
    fclose(fptr);
    return;
}

static void mallocConfig(Config **conf) {
    (*conf) = malloc(sizeof(Config));
    return;
}

void setupConfig(Config **confPtr) {
    mallocConfig(confPtr);
    Config *conf = *confPtr;
    FILE *fptr = fopen(".tectonical.config", "r");
    
    fscanf(fptr, "seed=%d\nwidth=%d\nheight=%d\nland-rate=%f\ntectonic-volatil\
ity=%d\ntectonic-impact-max-range=%d\ntectonic-impact-diminishing-factor=%f\n\
sea-plate-height=%f\nland-plate-height=%f\ntectonic-impact-factor=%f\nsea-lev\
el=%d\ngaussian-range=%d\ngaussian-diminishing-factor=%f\ntectonic-count=%d\n",
            &conf->seed, &conf->width, &conf->height, &conf->landRate, &conf->
            tectonicVolatility, &conf->tectonicImpactMaxRange, &conf->
            tectonicImpactDiminishingFactor, &conf->seaPlateHeight, &conf->
            landPlateHeight, &conf->tectonicImpactFactor, &conf->seaLevel, 
            &conf->gaussianRange, &conf->gaussianDiminishingFactor, &conf->
            tectonicCount);

    fclose(fptr);

    return;
}