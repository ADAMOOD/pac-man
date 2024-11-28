#include "game.h"
#include "map.h"
GameState GameTest(SDL_Renderer *renderer)
{
    int running = 1;
    SDL_Event event;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(renderer);
    Map map;
    if (GetMapFile(&map) != 0)
    {
        SDL_Log("Error loading map");
        return -1;
    }
    printMapInDetail(map);
    while (running)
    {
        // Zpracování událostí ve hře
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0; // end whole game
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            {
                running = 0; // Get back to menu
            }

            // game
        }
        if (MapShow(renderer, map) != 0)
        {
            FreeMap(&map);
            SDL_Log("rendering map ERR");
            return STATE_MENU;
        }
    }
    FreeMap(&map);
    return STATE_MENU;
}
