#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

typedef struct {
    int x, y;           
    int direction;      
    SDL_Texture *texture; 
} Player;

void init_player(Player *player, SDL_Renderer *renderer);
void update_player(Player *player);
void render_player(Player *player, SDL_Renderer *renderer);

#endif