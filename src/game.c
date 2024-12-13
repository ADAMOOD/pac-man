#include "game.h"
#include "map.h"
#include "player.h"

// Main game loop for testing game state
GameState GameTest(SDL_Renderer *renderer, double deltaTime)
{
    SDL_Event event;
    Map map;
    TTF_Font *font = TTF_OpenFont("PacFont.ttf", FBIG);
    if (!font)
    {
        SDL_Log("Error loading large font: %s", TTF_GetError());
        return STATE_MENU;
    }

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
        TTF_CloseFont(font);
        FreeMap(&map);
        return STATE_MENU;
    }
    double timeAccumulator = 0.0; // Accumulator to manage fixed time steps for player updates


   /* int textWidth, textHeight;
    TTF_SizeText(currentFont, menu_items[i], &textWidth, &textHeight);
    int w, h;
        SDL_GetRendererOutputSize(renderer, &w, &h);

        SDL_Rect location = {
            (w - textWidth) / 2,
            (h / 3) + i * (textHeight + 20),
            textWidth,
            textHeight
        };
        
        sdl_draw_text(renderer, currentFont, currentColor, location, menu_items[i]);*/

    while (1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
            {
                // Cleanup resources when exiting the loop
                FreeMap(&map);
                free_player(&player);
                TTF_CloseFont(font);
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
        renderMap(renderer, map);               // Render the map
        renderPlayer(renderer, &player, map); // Render the player
        
        SDL_RenderPresent(renderer); // Present the new frame
        SDL_Delay(16); // Limit the frame rate to approximately 60 FPS
    }
    // Cleanup resources when exiting the loop
    FreeMap(&map);
    free_player(&player);
    TTF_CloseFont(font);
    return STATE_MENU;
}
