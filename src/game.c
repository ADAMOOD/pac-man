#include "game.h"
#include "map.h"
#include "player.h"
GameState GameTest(SDL_Renderer *renderer, double deltaTime)
{
    SDL_Event event;
    Map map;
    if (GetMapFile(&map) != 0)
    {
        SDL_Log("Error loading map");
        return -1;
    }

    Player player;
    if (init_player(&player, map) == 1)
    {
        SDL_Log("Error initializing player");
        FreeMap(&map);
        return -1;
    }

    const double playerSpeed = 5.0;
    double timeAccumulator = 0.0;

    while (1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
            {
                FreeMap(&map);
                return STATE_MENU;
            }
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    player.direction = UP; // Nastavení směru na nahoru
                    break;
                case SDLK_DOWN:
                    player.direction = DOWN; // Nastavení směru na dolů
                    break;
                case SDLK_LEFT:
                    player.direction = LEFT; // Nastavení směru na doleva
                    break;
                case SDLK_RIGHT:
                    player.direction = RIGHT; // Nastavení směru na doprava
                    break;
                default:
                    break;
                }
            }

            // Aktualizace logiky hráče na základě deltaTime
            timeAccumulator += deltaTime;
            while (timeAccumulator > 1.0 / playerSpeed)
            {
                movePlayer(&player, &map);
                timeAccumulator -= 1.0 / playerSpeed;
                SDL_Log("Player moved to x: %d, y: %d", player.x, player.y);
            }

            // Vykreslení mapy a hráče
            if (MapShow(renderer, map) != 0)
            {
                FreeMap(&map);
                SDL_Log("Rendering map error");
                return STATE_MENU;
            }

            SDL_RenderPresent(renderer);
            SDL_Delay(16); // Omezení na 60 FPS
        }

        FreeMap(&map);
        return STATE_MENU;
    }
