#ifndef TECTONICAL_CONFIG_H
#define TECTONICAL_CONFIG_H

typedef struct {
    int seed;
    int width;
    int height;
    float landRate;
    int tectonicVolatility;
    int tectonicImpactMaxRange;
    float tectonicImpactDiminishingFactor;
    float seaPlateHeight;
    float landPlateHeight;
    float tectonicImpactFactor;
    float seaLevel;
    int gaussianRange;
    float gaussianDiminishingFactor;
    int tectonicCount;
} Config;

/* ======== config.c ======== */

extern void checkConfig();

extern void setupConfig(Config **confPtr);

#endif /* TECTONICAL_CONFIG_H */