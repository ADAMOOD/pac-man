#pragma once

#include <SDL2/SDL.h>
#include "map.h"

typedef struct {
    int x, y;             // Logická pozice hráče na mapě
    double renderX, renderY; // Vykreslená (interpolovaná) pozice
    Direction direction;  // Směr pohybu hráče
    SDL_Texture *texture; // Textura hráče
} Player;

int init_player(Player *player, SDL_Renderer *renderer, Map map);
int getPlayerLocation(Map map,int *x,int *y);
int movePlayer(Player *player, Map *map);
void changeDirection(SDL_Keycode key,Player *player,Map map);
void free_player(Player *player);
void renderPlayer(SDL_Renderer *renderer, Player *player,Map m);
void updatePlayerRenderPosition(Player *player, double deltaTime, double speed);