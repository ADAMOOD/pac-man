#pragma once

#include <SDL2/SDL.h>

typedef struct {
    int x, y;          
    int direction;      
    SDL_Texture *texture; 
} Ghost;

void init_ghost(Ghost *ghost, SDL_Renderer *renderer);
//void update_ghost(Ghost *ghost, Player *player);
void render_ghost(Ghost *ghost, SDL_Renderer *renderer);
