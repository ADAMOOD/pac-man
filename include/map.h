#pragma once
#include "universal.h"
#include <string.h>

typedef struct {
    char **data;
    int rows;
    int cols;
    int tele1[2],tele0[2];
} Map;

#define SMALLPEARL 10;
#define BIGPEARL 50;


#define MAP_PATH "../assets/map.txt"
int renderMap(SDL_Renderer *renderer, Map m);
int GetMapFile(Map *map);
void FreeMap(Map *map);
int getFileRowsAndCols(FILE *file, int *cols, int *rows);
Map allocateMap(int rows, int cols) ;
void getDatafromFile(FILE *f, Map *m);
int mapIsInvalid(Map map);
void printMapInDetail(Map m);
void getMapMesurements(Map map,int w,int h, int *wallPartSizeW,int *wallPartSizeH,int *marginX,int *marginY);
int fingOnMap(Map map,char c, int *x, int *y);
void showMap(Map map);
int checkForPearlsLeft(Map map);
Map deepCopyMap(const Map *original);
