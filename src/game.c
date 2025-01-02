#include "game.h"
#include "map.h"
#include "player.h"
#include "uiInfo.h"
#include "ghost.h"

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
    GameState gameState = GAME_RUNNING;
    double pauseTimeAccumulator = 0.0;
    double playerTimeAccumulator = 0;
    double ghostTimeAccumulator = 0;
    int running = 1;
    double eating = 0.0;
    Uint64 previousTime = SDL_GetPerformanceCounter();
    while (running)
    {
        // mesure time by the last frame
        Uint64 currentTime = SDL_GetPerformanceCounter();
        deltaTime = (double)(currentTime - previousTime) / SDL_GetPerformanceFrequency();
        previousTime = currentTime;
        if (checkForPearlsLeft(map) == 0)
        {
            SDL_RenderClear(renderer);
            char combinedString[256];
            sprintf(combinedString, "%s%d", "YOU WON! ", player.score);
            sdl_draw_text_center(renderer, combinedString);
            SDL_RenderPresent(renderer);
            SDL_Delay(3000);
            running = 0;
            continue;
        }
        //input handeling
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
            {
                running=0;
                continue;
            }
            playerChangeDirection(event.key.keysym.sym, &player, map);
        }
        
        if (gameState == GAME_PAUSED)
        {
            pauseTimeAccumulator -= deltaTime;
            if (pauseTimeAccumulator <= 0.0)
            {
                gameState = GAME_RUNNING;
            }

            continue; // Přeskočíme aktualizace herní logiky
        }
        //increese of the acumulator as the new iteration of the cycle is going
        playerTimeAccumulator += deltaTime;
        ghostTimeAccumulator += deltaTime;

        // player actualization
        while (playerTimeAccumulator > 1.0 / player.speed)
        {
            switch (movePlayer(&player, &map))
            {
            case 3: // PACMAN ATE BIG PEARL
                for (int i = 0; i < 4; i++)
                    ghosts[i].state = EATEABLE;
                eating = 0;
                break;
            case 2: // PACMAN MEETS A GHOST
            {
                Ghost *meetedGhost = getGhostById(ghosts, 4, getNextCell(player, map));
                switch (meetedGhost->state)
                {
                case EATEABLE:
                    findAWayHome(meetedGhost, map);
                    meetedGhost->state = EATEN;
                    player.score += 100;
                    break;
                case HUNTING:
                    playerLostLife(&player, pacInitDirection, pacInitX, pacInitY, &map,&gameState,&pauseTimeAccumulator);
                    if (player.lives <= 0)
                        running = 0;
                    continue;
                }
                break;
            }
            default:
                break;
            }
            playerTimeAccumulator -= 1.0 / player.speed;
        }

        // ghosts actualization
        while (ghostTimeAccumulator > 1.0 / ghosts[0].speed)
        {
            for (int i = 0; i < 4; i++)
            {
                if (moveGhost(&ghosts[i], &map) == 2 && ghosts[i].state == HUNTING)
                {
                    playerLostLife(&player, pacInitDirection, pacInitX, pacInitY, &map,&gameState,&pauseTimeAccumulator);
                    if (player.lives <= 0)
                    {
                        running = 0;
                    }
                    continue;
                }
            }
            ghostTimeAccumulator -= 1.0 / ghosts[0].speed; 
        }

        if (ghosts[0].state == EATEABLE ||
            ghosts[1].state == EATEABLE ||
            ghosts[2].state == EATEABLE ||
            ghosts[3].state == EATEABLE)
        {
            eating += deltaTime * 1000;
        }
        if (eating > 700000)
        {
            for (int i = 0; i < 4; i++)
            {
                ghosts[i].state = HUNTING;
                freeWayHome(&ghosts[i]);
            }
        }
        if (!running)
        {
            break; // Exit the loop after all updates
        }

        // Updates the player's animation and smoothly interpolates their position
        updatePlayer(&player, deltaTime);

        // all the ghosts
        for (int i = 0; i < 4; i++)
        {
            renderGhost(renderer, &ghosts[i], map);
            updateGhost(&ghosts[i], deltaTime);
        }

        // RENDERING PHASE
        SDL_RenderClear(renderer); // Clear the screen

        renderMap(renderer, map);

        renderPlayer(renderer, &player, map); // Render the player

        for (int i = 0; i < 4; i++)
        {
            renderGhost(renderer, &ghosts[i], map);
        }

        renderUI(player, map, renderer);

        SDL_RenderPresent(renderer); // Present the new frame !! important

    }
    saveBestScore(player);

    FreeMap(&map);
    free_player(&player);
    for (int i = 0; i < 4; i++)
    {
        free_ghost(&ghosts[i]);
    }
    /* Mix_FreeMusic(background_music); // Uvolnění hudby po jejím přehrání
     Mix_CloseAudio();
     Mix_Quit(); // Ukončení SDL_mixer*/
    return STATE_MENU;
}

void playerLostLife(Player *player, Direction pacInitDirection, int pacInitX, int pacInitY, Map *map,GameState *gameState,double *pauseTimeAccumulator)
{
    player->lives--;
    player->direction = pacInitDirection;
    movePlayerTo(pacInitX, pacInitY, player, map);

    // game is stopped for 3 sec
    *gameState = GAME_PAUSED;
    *pauseTimeAccumulator = 3.0; // 3 sekundy pauzy
}