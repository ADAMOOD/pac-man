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
        return;
    }

    // Try to render the texture
    if (SDL_RenderCopy(renderer, texture, NULL, &location) != 0) {
        SDL_Log("Failed to render text texture: %s", SDL_GetError());
        SDL_DestroyTexture(texture);  // Ensure the texture is destroyed even on failure.
        return;
    }

    SDL_DestroyTexture(texture);  // Always destroy the texture after usage.
}


void RenderFilledCircle(SDL_Renderer *renderer, int x, int y, int radius)
{
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w; // Horizontal distance to circle's center
            int dy = radius - h; // Vertical distance to circle's center
            if ((dx * dx + dy * dy) <= (radius * radius)) // Check if inside circle
            {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}