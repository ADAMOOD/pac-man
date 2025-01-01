#pragma once

#include <SDL2/SDL.h>
#include "map.h"

typedef struct {
    int x, y;                    // Logická pozice hráče na mapě
    double renderX, renderY;     // Vykreslená (interpolovaná) pozice
    Direction direction;         // Směr pohybu hráče
    double speed;                // Rychlost hráče
    SDL_Texture *texture;        // Textura pro celou postavu (sprite sheet)
    int currentFrame;            // Aktuální frame animace
    double frameTime;            // Čas pro každý frame
    double timeAccumulator;      // Akumulátor času pro animaci
    int frameWidth, frameHeight; // Rozměry jednoho frame
    int totalFrames;             // Počet celkových snímků animace
    int score;
    int lives;
} Player;

int init_player(Player *player, SDL_Renderer *renderer, Map map);
int movePlayer(Player *player, Map *map);
void playerChangeDirection(SDL_Keycode key,Player *player,Map map);
void free_player(Player *player);
void renderPlayer(SDL_Renderer *renderer, Player *player,Map m);
void updatePlayerRenderPosition(Player *player, double deltaTime);
void updatePlayerAnim(Player *player, double deltaTime);
void updatePlayer(Player *player, double deltaTime);
int saveBestScore(Player player);
void movePlayerTo(int x, int y, Player *player, Map *map);
char getNextCell(Player player, Map map);