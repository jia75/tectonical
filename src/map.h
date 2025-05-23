typedef struct {
    float **map;
    int height;
    int width;
} Map;

extern void freeMap(Map *map);

extern void mallocMap(Map **mapPtr, int height, int width);

extern void clearMap(Map *map);