#ifndef UNIVERSAL
#define UNIVERSAL
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
typedef enum
{
    STATE_MENU,
    STATE_GAME,
    STATE_EXIT
} GameState;
#define SCREENWIDTH 800
#define SCREENHEIGHT 800
#endif