#include "game.h"
#include "map.h"
GameState GameTest(SDL_Renderer *renderer) {
    int running = 1;
    SDL_Event event;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Nastavení černé barvy pro pozadí
    SDL_RenderClear(renderer);
    
    while (running) {
        // Zpracování událostí ve hře
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;  // Ukončení celé hry
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = 0;  // Návrat do menu
            }

            // Další obsluha herních událostí zde...
        }
       MapShow(renderer);
    }
    return STATE_MENU;
}
