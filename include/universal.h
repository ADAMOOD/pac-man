#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
typedef enum
{
    STATE_MENU,
    STATE_GAME,
    STATE_EXIT
} GameState;

typedef enum
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
} Direction;
#define SCREENWIDTH 800
#define SCREENHEIGHT 800
