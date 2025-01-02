#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>  // For rand() and srand()
#include <time.h>    // For time() to seed the random number generator
typedef enum
{
    GAME_RUNNING,
    GAME_PAUSED,
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

int isCharInArray(char ch, const char *characters);