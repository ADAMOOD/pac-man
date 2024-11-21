#include "map.h"

void MapShow(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);

    SDL_Color color = {255, 255, 255, 255};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); // Nastavení kreslicí barvy

    int margin = 100; // Odsazení
    SDL_Rect rect = {
        margin,         // X: 10 pixelů od levého okraje
        margin,         // Y: 10 pixelů od horního okraje
        w - 2 * margin, // Šířka: Šířka obrazovky mínus 20 pixelů (10 zleva a 10 zprava)
        h - 2 * margin  // Výška: Výška obrazovky mínus 20 pixelů (10 shora a 10 zdola)
    };
    SDL_RenderDrawRect(renderer, &rect); // Kreslení obrysu čtyřúhelníku

    SDL_RenderPresent(renderer); // Aktualizace obrazovky
                                 /* for (int i = 0; i < 10; i++)
                                  {
                                      SDL_Rect rect2 = {100 * i + 1, 10, 10, 10};
                                      SDL_RenderFillRect(renderer, &rect2);
                                      SDL_RenderPresent(renderer);
                                  }*/
}
