#include "universal.h"
#include "drawing.h"
#include "menu.h"

void sdl_draw_text(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, SDL_Rect location, const char *text);

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    if (TTF_Init() == -1)
    {
        fprintf(stderr, "TTF_Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("SDL experiments", 100, 100, SCREENHEIGHT, SCREENWIDTH, SDL_WINDOW_SHOWN);
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
    int in_menu = 1;
    SDL_Event event;
    int selectedIndex = 0;
while (running) {
        // Event handling
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            int result = menu_handle_event(&event, &selectedIndex);
            if (result != MENU_CONTINUE) {
                running = 0;
                // Provést požadovanou akci podle výběru
            }
        }

        // Renderování menu s aktuálně vybranou položkou
        menu_render(renderer, selectedIndex);

        // Aktualizace obrazovky
        SDL_RenderPresent(renderer);
    }


    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
