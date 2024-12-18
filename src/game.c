#include "game.h"
#include "map.h"
#include "player.h"
#include "uiInfo.h"
#include "ghost.h"

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
    int pacInitX = player.x, pacInitY = player.y, pacInitDirection = player.direction;
    SDL_Log("rows %d cols %d", map.rows, map.cols);
    Ghost ghosts[4];
    if (init_ghost(&ghosts[0], renderer, "../assets/BlinkySpriteSheet.png", map, 'b', RANDOM, player) == 1)
    {
        SDL_Log("Error initializing blinky");
        FreeMap(&map);
        free_player(&player);
        return STATE_MENU;
    }
    if (init_ghost(&ghosts[1], renderer, "../assets/PinkySpriteSheet.png", map, 'n', RANDOM, player) == 1)
    {
        SDL_Log("Error initializing pinky");
        FreeMap(&map);
        free_player(&player);
        free_ghost(&ghosts[0]);
        return STATE_MENU;
    }
    if (init_ghost(&ghosts[2], renderer, "../assets/InkySpriteSheet.png", map, 'i', RANDOM, player) == 1)
    {
        SDL_Log("Error initializing inky");
        FreeMap(&map);
        free_player(&player);
        free_ghost(&ghosts[0]);
        free_ghost(&ghosts[1]);
        return STATE_MENU;
    }
    if (init_ghost(&ghosts[3], renderer, "../assets/ClydeSpriteSheet.png", map, 'c', RANDOM, player) == 1)
    {
        SDL_Log("Error initializing clyde");
        FreeMap(&map);
        free_player(&player);
        free_ghost(&ghosts[0]);
        free_ghost(&ghosts[1]);
        free_ghost(&ghosts[2]);
        return STATE_MENU;
    }

    double timeAccumulator = 0.0; // Accumulator to manage fixed time steps for player updates
    int x, y;
    int running = 1;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
            {
                saveBestScore(player);
                // Cleanup resources when exiting the loop
                FreeMap(&map);
                free_player(&player);
                free_ghost(&ghosts[0]);
                free_ghost(&ghosts[1]);
                free_ghost(&ghosts[2]);
                free_ghost(&ghosts[3]);

                return STATE_MENU;
            }
            playerChangeDirection(event.key.keysym.sym, &player, map);
        }
        // Update player logic
        timeAccumulator += deltaTime;
        while (timeAccumulator > 1.0 / player.speed)
        {
            if (moveAllTheGosts(ghosts, 4, &map) == 2)
            {
                player.lives--;
                if (player.lives <= 0)
                {
                    running = 0;
                }
                player.direction = pacInitDirection;
                movePlayerTo(pacInitX, pacInitY, &player, &map);
                SDL_Delay(3000);
                continue;
            }
            movePlayer(&player, &map);
            timeAccumulator -= 1.0 / player.speed;
        }

        if (!running)
        {
            break; // Exit the loop after all updates
        }
        // Updates the player's animation and smoothly interpolates their position
        updatePlayer(&player, deltaTime);
        updateGhost(&ghosts[0],deltaTime);
        updateGhost(&ghosts[1],deltaTime);
        updateGhost(&ghosts[2],deltaTime);
        updateGhost(&ghosts[3],deltaTime);

        // **Rendering phase**
        SDL_RenderClear(renderer);            // Clear the screen
        renderMap(renderer, map);             // Render the map
        renderPlayer(renderer, &player, map); // Render the player
        renderGhost(renderer, &ghosts[0], map);
        renderGhost(renderer, &ghosts[1], map);
        renderGhost(renderer, &ghosts[2], map);
        renderGhost(renderer, &ghosts[3], map);
        renderUI(player, map, renderer);

        SDL_RenderPresent(renderer); // Present the new frame
        SDL_Delay(16);               // Limit the frame rate to approximately 60 FPS
    }
    // Cleanup resources when exiting the loop
    FreeMap(&map);
    free_player(&player);
    free_ghost(&ghosts[0]);
    free_ghost(&ghosts[1]);
    free_ghost(&ghosts[2]);
    free_ghost(&ghosts[3]);
    return STATE_MENU;
}
