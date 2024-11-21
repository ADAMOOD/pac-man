#ifndef MAP_H
#define MAP_h
#include "universal.h"

typedef enum
{
    MAPWALLH,
    MAPWALLV,
} MapStructures;
void MapShow(SDL_Renderer *renderer);
#endif