#pragma once
#include "universal.h"
#include "drawing.h"
#include "player.h"
#include "map.h"
#include <SDL2/SDL.h>

typedef enum
{
    RANDOM
} Movements;

typedef enum
{
    HUNTING,
    EATEABLE,
} GHOSTSTATE;

typedef struct
{
    int x, y, homeX, homeY;      // Logická pozice hráče na mapě
    double renderX, renderY;     // Vykreslená (interpolovaná) pozice
    Direction direction;         // Směr pohybu hráče
    double speed;                // Rychlost hráče
    SDL_Texture *texture;        // Textura pro celou postavu (sprite sheet)
    SDL_Texture *textureBlue;    // Textura pro celou postavu (sprite sheet)
    int currentFrame;            // Aktuální frame animace
    double frameTime;            // Čas pro každý frame
    double timeAccumulator;      // Akumulátor času pro animaci
    int frameWidth, frameHeight; // Rozměry jednoho frame
    int totalFrames;             // Počet celkových snímků animace
    char id;
    Movements movement;
    GHOSTSTATE state;
    char lastCell;
    int isMooving;
} Ghost;

int init_ghost(Ghost *ghost, SDL_Renderer *renderer, char *texture, Map map, char indentifier, Movements movement, Player player);
void changeDirection(SDL_Keycode key, Ghost *ghost, Map map);
void renderGhost(SDL_Renderer *renderer, Ghost *ghost, Map m);
int moveGhost(Ghost *ghost, Map *map);
void updateGhostRenderPosition(Ghost *ghost, double deltaTime);
void updateGhostAnim(Ghost *ghost, double deltaTime);
void free_ghost(Ghost *ghost);
void updateGhost(Ghost *ghost, double deltaTime);
void setRandomDirection(Ghost *ghost);
int moveAllTheGosts(Ghost *ghost, int count, Map *map);
