#pragma once

#include <SDL2/SDL.h>
#include "map.h"

typedef struct {
    int x, y;           
    Direction direction;      
} Player;

int init_player(Player *player,Map map);
int getPlayerLocation(Map map,int *x,int *y);
int movePlayer(Player *player, Map *map);