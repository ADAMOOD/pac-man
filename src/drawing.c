#include "drawing.h"

void sdl_draw_text(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, SDL_Rect location, const char* text) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, &location);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
