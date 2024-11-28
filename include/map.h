#ifndef MAP_H
#define MAP_h
#include "universal.h"
#include <string.h>



typedef enum
{
    MAPWALLH,
    MAPWALLV,
} MapStructures;

typedef struct {
    char **data;
    int rows;
    int cols;
} Map;

#define MAP_PATH "../assets/map.txt"
int MapShow(SDL_Renderer *renderer, Map m);
int GetMapFile(Map *map);
void FreeMap(Map *map);
int getFileRowsAndCols(FILE *file, int *cols, int *rows);
Map *initializeMap(int rows, int cols) ;
void getDatafromFile(FILE *f, Map *m);
int mapIsInvalid(Map map);
void printMapInDetail(Map m);
#endif