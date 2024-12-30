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
    // Inicializace SDL a SDL_mixer
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return -1;
    }

    /*if (Mix_Init(MIX_INIT_MP3) == 0)
    {
        SDL_Log("Mix_Init failed: %s", Mix_GetError());
        return -1;
    }

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        SDL_Log("Mix_OpenAudio failed: %s", Mix_GetError());
        return -1;
    }

    // Načítání hudby
    Mix_Music *background_music = Mix_LoadMUS("../assets/bacgroundMusici.mp3");
    if (background_music == NULL)
    {
        SDL_Log("Error loading background music: %s", Mix_GetError());
        return -1;
    }

    // Přehrávání hudby na smyčku (-1)
    if (Mix_PlayMusic(background_music, -1) == -1)
    {
        SDL_Log("Error playing background music: %s", Mix_GetError());
        return -1;
    }*/

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
    double eating = 0.0;
    while (running)
    {
        if (checkForPearlsLeft(map) == 0)
        {
            SDL_Log("Vyhra");

            // Vyčistit obrazovku a vykreslit text
            SDL_RenderClear(renderer);

            char combinedString[256]; // Dostatečně velké pole pro výsledný řetězec

            // Spojení obou řetězců
            sprintf(combinedString, "%s%d", "YOU WON! ",player.score);
            sdl_draw_text_center(renderer, combinedString);
            SDL_RenderPresent(renderer); // Aktualizovat obrazovku

            SDL_Delay(3000);

            running = 0;
            continue;
        }
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
               /* Mix_FreeMusic(background_music); 
                Mix_CloseAudio();
                Mix_Quit();*/
                return STATE_MENU;
            }
            playerChangeDirection(event.key.keysym.sym, &player, map);
        }
        // Update player logic
        timeAccumulator += deltaTime;
        while (timeAccumulator > 1.0 / player.speed)
        {
            if (moveGhost(&ghosts[0], &map) == 2 && ghosts[0].state == HUNTING ||
                moveGhost(&ghosts[1], &map) == 2 && ghosts[1].state == HUNTING ||
                moveGhost(&ghosts[2], &map) == 2 && ghosts[0].state == HUNTING ||
                moveGhost(&ghosts[3], &map) == 2 && ghosts[0].state == HUNTING)
            {
                Mix_HaltMusic();
                player.lives--;
                if (player.lives <= 0)
                {
                    running = 0;
                }
                player.direction = pacInitDirection;
                movePlayerTo(pacInitX, pacInitY, &player, &map);
                SDL_Delay(3000);
               /* if (Mix_PlayMusic(background_music, -1) == -1)
                {
                    SDL_Log("Error playing background music: %s", Mix_GetError());
                    return -1;
                }*/
                continue;
            }
            if (movePlayer(&player, &map) == 3)
            {
                ghosts[0].state = EATEABLE;
                ghosts[1].state = EATEABLE;
                ghosts[2].state = EATEABLE;
                ghosts[3].state = EATEABLE;
                eating = 0;
            }
            timeAccumulator -= 1.0 / player.speed;
        }

        if (ghosts[0].state == EATEABLE ||
            ghosts[1].state == EATEABLE ||
            ghosts[2].state == EATEABLE ||
            ghosts[3].state == EATEABLE)
        {
            eating += deltaTime * 1000;
        }
        if (eating > 5000)
        {
            ghosts[0].state = HUNTING;
            ghosts[1].state = HUNTING;
            ghosts[2].state = HUNTING;
            ghosts[3].state = HUNTING;
        }
        if (!running)
        {
            break; // Exit the loop after all updates
        }
        // Updates the player's animation and smoothly interpolates their position
        updatePlayer(&player, deltaTime);
        // all the ghosts
        updateGhost(&ghosts[0], deltaTime);
        updateGhost(&ghosts[1], deltaTime);
        updateGhost(&ghosts[2], deltaTime);
        updateGhost(&ghosts[3], deltaTime);

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
   /* Mix_FreeMusic(background_music); // Uvolnění hudby po jejím přehrání
    Mix_CloseAudio();
    Mix_Quit(); // Ukončení SDL_mixer*/
    return STATE_MENU;
}
