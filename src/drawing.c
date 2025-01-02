#include "drawing.h"


void sdl_draw_text(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, SDL_Rect location, const char *text)
{
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
    if (surface == NULL)
    {
        SDL_Log("Failed to create text surface: %s", TTF_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); // Free the surface as soon as the texture is created.

    if (texture == NULL)
    {
        SDL_Log("Failed to create text texture: %s", SDL_GetError());
        return;
    }

    // Try to render the texture
    if (SDL_RenderCopy(renderer, texture, NULL, &location) != 0)
    {
        SDL_Log("Failed to render text texture: %s", SDL_GetError());
        SDL_DestroyTexture(texture); // Ensure the texture is destroyed even on failure.
        return;
    }

    SDL_DestroyTexture(texture); // Always destroy the texture after usage.
}



void sdl_draw_text_center(SDL_Renderer *renderer, const char *text)
{
    SDL_Color color = {255, 255, 255, 255}; // Bílá barva textu
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);

    int fontSize = 100; // Počáteční velikost písma
    TTF_Font *font = NULL;
    int textWidth = 0, textHeight = 0;

    // Najít správnou velikost písma
    while (fontSize > 0)
    {
        font = TTF_OpenFont("pixelated.ttf", fontSize);
        if (!font)
        {
            SDL_Log("Error loading font: %s", TTF_GetError());
            return;
        }

        TTF_SizeText(font, text, &textWidth, &textHeight);

        if (textWidth <= w) // Pokud text vejde do šířky obrazovky
        {
            break;
        }

        TTF_CloseFont(font);
        font = NULL;
        fontSize--; // Snížit velikost písma a opakovat
    }

    if (!font)
    {
        SDL_Log("Failed to find suitable font size.");
        return;
    }

    SDL_Rect location = {
        (w - textWidth) / 2,
        (h / 3) + (textHeight + 20),
        textWidth,
        textHeight};

    sdl_draw_text(renderer, font, color, location, text);

    TTF_CloseFont(font);
}

void RenderFilledCircle(SDL_Renderer *renderer, int x, int y, int radius)
{
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w;                          // Horizontal distance to circle's center
            int dy = radius - h;                          // Vertical distance to circle's center
            if ((dx * dx + dy * dy) <= (radius * radius)) // Check if inside circle
            {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}