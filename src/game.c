#include "game.h"
#include "map.h"
#include "player.h"
GameState GameTest(SDL_Renderer *renderer,double deltaTime)
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
    // printMapInDetail(map);
    Player player;
    if (init_player(&player, map) == 1)
    {
        SDL_Log("Error initializing player");
        return -1;
    }
    SDL_Log("x %d y %d",player.x,player.y);
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
            
            movePlayer(&player,&map);
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
