#include "universal.h"
#include "drawing.h"
#include "menu.h"
#include "game.h"

void sdl_draw_text(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, SDL_Rect location, const char *text);

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1)
    {
        fprintf(stderr, "TTF_Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("SDL experiments", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREENWIDTH, SCREENHEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    TTF_Font *font = TTF_OpenFont("PacFont.ttf", 50);

    if (!font)
    {
        fprintf(stderr, "TTF_OpenFont Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        TTF_Quit();
        return 1;
    }
    SDL_Color txtcolor = {255, 0, 0, 255};
    SDL_Rect rect = {100, 200, 500, 200};

    int running = 1;
    SDL_Event event;
    int selectedIndex = 0;
    int result = MENU_CONTINUE;
    GameState state = STATE_MENU;
    Uint64 last = SDL_GetPerformanceCounter();
    while (running)
    {
        Uint64 now = SDL_GetPerformanceCounter();
        double deltaTime = (double)((now - last) / (double)SDL_GetPerformanceFrequency());
        
        // Zpracování událostí
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0; // Ukončení celé aplikace
            }

            if (state == STATE_MENU)
            {
                int result = menu_handle_event(&event, &selectedIndex);

                if (result == MENU_EXIT)
                {
                    running = 0; // Ukončení aplikace
                }
                else if (result == MENU_START_GAME)
                {
                    state = STATE_GAME; // Přepnout do herního režimu
                }
            }
            else if (state == STATE_GAME)
            {
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                {
                    state = STATE_MENU; // Návrat do menu při stisku Escape
                }
                // Další zpracování herních událostí, pokud je potřeba
            }
        }
        // Vykreslování
        SDL_RenderClear(renderer);
        if (state == STATE_EXIT)
        {
            running = 0; // Ukončení aplikacea
        }
        if (state == STATE_MENU)
        {
            menu_render(renderer, selectedIndex);
        }
        else if (state == STATE_GAME)
        {
            state = GameTest(renderer,deltaTime); // Vykreslení herní obrazovky
        }
        SDL_RenderPresent(renderer); // Aktualizace obrazovky
        last = now;
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
