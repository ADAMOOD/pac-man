#include "menu.h"
#include "drawing.h"

void menu_render(SDL_Renderer *renderer, int selected_index)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Nastavení černé barvy pro pozadí
    SDL_RenderClear(renderer); // Vyčištění obrazovky
    //dulezite

    SDL_Color normal_color = {255, 255, 255, 255};   // Bílá pro normální položky
    SDL_Color selected_color = {0, 255, 0, 255};     // Zelená pro vybranou položku
    const char *menu_items[] = {"PAC-MAN", "Start Game", "Exit"};
    int num_items = 3;

    TTF_Font* largeFont = TTF_OpenFont("PacFont.ttf", FLARGE);
    TTF_Font* bigFont = TTF_OpenFont("PacFont.ttf", FBIG);
    if (!largeFont || !bigFont) {
        SDL_Log("Error loading large font: %s", TTF_GetError());
        return;
    }

    for (int i = 0; i < num_items; ++i)
    {
        TTF_Font* currentFont = (i == 0) ? largeFont : bigFont;

        // Určení barvy podle toho, zda je položka vybraná
        SDL_Color currentColor = (i-1 == selected_index) ? selected_color : normal_color;

        int textWidth, textHeight;
        TTF_SizeText(currentFont, menu_items[i], &textWidth, &textHeight);
        int w, h;
        SDL_GetRendererOutputSize(renderer, &w, &h);

        SDL_Rect location = {
            (w - textWidth) / 2,
            (h / 3) + i * (textHeight + 20),
            textWidth,
            textHeight
        };
        
        sdl_draw_text(renderer, currentFont, currentColor, location, menu_items[i]);
    }
    TTF_CloseFont(largeFont);
    TTF_CloseFont(bigFont);
}

int menu_handle_event(SDL_Event *event, int *selected_index)
{
    if (event->type == SDL_KEYDOWN)
    {
        if (event->key.keysym.sym == SDLK_RETURN)  // Enter pro výběr
        {
            if (*selected_index == 0) {
                //SDL_Log("%ls",selected_index);
                return MENU_START_GAME;  // Start Game
            } else if (*selected_index == 1) {
                //SDL_Log("%ls",selected_index);
                return MENU_EXIT;  // Exit
            }
        }
        else if (event->key.keysym.sym == SDLK_ESCAPE) 
        {
            //return MENU_EXIT;// muze byt pro implementaci pozdeji nejakeho pouse
        }
        else if (event->key.keysym.sym == SDLK_UP) 
        {
            if (*selected_index > 0) {
                (*selected_index)--;  // Snížení indexu pro výběr
            }
        }
        else if (event->key.keysym.sym == SDLK_DOWN) 
        {
            if (*selected_index < 1) {
                (*selected_index)++;  // Zvýšení indexu pro výběr
            }
        }
    }
    return MENU_CONTINUE;
}