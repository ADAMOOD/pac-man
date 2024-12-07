#pragma once
#include "universal.h"
#define FMEDIUM 25
#define FSMALL 15
#define FBIG 35
#define FLARGE 60

void sdl_draw_text(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, SDL_Rect location, const char* text);
void RenderFilledCircle(SDL_Renderer *renderer, int x, int y, int radius);

