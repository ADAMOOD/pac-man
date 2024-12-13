#include "game.h"
#include "map.h"
#include "player.h"
#include "uiInfo.h"

// Main game loop for testing game state
GameState GameTest(SDL_Renderer *renderer, double deltaTime)
{
    SDL_Event event;
    Map map;

    // Load the map file
    if (GetMapFile(&map) != 0)
    {
        SDL_Log("Error loading map");
        return STATE_MENU;
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    Player player;

    // Initialize player with its texture and initial position
    if (init_player(&player, renderer, map) == 1)
    {
        SDL_Log("Error initializing player");
        FreeMap(&map);
        return STATE_MENU;
    }
    double timeAccumulator = 0.0; // Accumulator to manage fixed time steps for player updates
    int x,y;
    while (1)
    {
        SDL_Log("0 nalezena %d",fingOnMap(map,'0',&x,&y));//funkce vraci 0 pokud se '0' najde. dokud se neteleportuje tak '0' ale jakmile se teleportuje tak ji jakoby prepise a uz tam neni
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
            {
                // Cleanup resources when exiting the loop
                FreeMap(&map);
                free_player(&player);
                return STATE_MENU;
            }
            changeDirection(event.key.keysym.sym, &player, map);
        }
        // Update player logic
        timeAccumulator += deltaTime;
        while (timeAccumulator > 1.0 / player.speed)
        {
            movePlayer(&player, &map);
            timeAccumulator -= 1.0 / player.speed;
        }
        // Interpolate player's position for smooth rendering
        updatePlayerRenderPosition(&player, deltaTime);
        // **Rendering phase**
        SDL_RenderClear(renderer);            // Clear the screen
        renderMap(renderer, map);             // Render the map
        renderPlayer(renderer, &player, map); // Render the player
        renderUI(player,map,renderer);

        SDL_RenderPresent(renderer); // Present the new frame
        SDL_Delay(16);               // Limit the frame rate to approximately 60 FPS
    }
    // Cleanup resources when exiting the loop
    FreeMap(&map);
    free_player(&player);
    return STATE_MENU;
}
