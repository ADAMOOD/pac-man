#include "drawing.h"

void sdl_draw_text(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, SDL_Rect location, const char* text) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    if (surface == NULL) {
        SDL_Log("Failed to create text surface: %s", TTF_GetError());
        return;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);  // Free the surface as soon as the texture is created.
    if (texture == NULL) {
        SDL_Log("Failed to create text texture: %s", SDL_GetError());
        return;  // Stop further operations if texture creation fails.
    }
    if (SDL_RenderCopy(renderer, texture, NULL, &location) != 0) {
        SDL_Log("Failed to render text texture: %s", SDL_GetError());
    }
    SDL_DestroyTexture(texture);  // Always destroy the texture to avoid leaks.
}