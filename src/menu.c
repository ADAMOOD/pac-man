#include "menu.h"
#include "drawing.h"

void menu_render(SDL_Renderer *renderer, int selected_index)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Nastavení černé barvy pro pozadí
    SDL_RenderClear(renderer);                      // Vyčištění obrazovky
    // dulezite
    int bestScore=getBestScore();
    SDL_Color normal_color = {255, 255, 255, 255}; // Bílá pro normální položky
    SDL_Color selected_color = {0, 255, 0, 255};   // Zelená pro vybranou položku
    const char *menu_items[] = {"PAC-MAN", "Start Game", "Exit"};
    int num_items = 3;

    TTF_Font *largeFont = TTF_OpenFont("PacFont.ttf", FLARGE);
    if (!largeFont)
    {
        SDL_Log("Error loading large font: %s", TTF_GetError());
        return;
    }
    TTF_Font *bigFont = TTF_OpenFont("PacFont.ttf", FBIG);
    if (!bigFont)
    {
        SDL_Log("Error loading big font: %s", TTF_GetError());
        TTF_CloseFont(largeFont);
        return;
    }
    TTF_Font *smallFont = TTF_OpenFont("pixelated.ttf", FSMALL);
    if (!smallFont)
    {
        SDL_Log("Error loading small font: %s", TTF_GetError());
        TTF_CloseFont(largeFont);
        TTF_CloseFont(bigFont);
        return;
    }

    for (int i = 0; i < num_items; ++i)
    {
        TTF_Font *currentFont = (i == 0) ? largeFont : bigFont;

        // Určení barvy podle toho, zda je položka vybraná
        SDL_Color currentColor = (i - 1 == selected_index) ? selected_color : normal_color;

        int textWidth, textHeight;
        TTF_SizeText(currentFont, menu_items[i], &textWidth, &textHeight);
        int w, h;
        SDL_GetRendererOutputSize(renderer, &w, &h);

        SDL_Rect location = {
            (w - textWidth) / 2,
            (h / 3) + i * (textHeight + 20),
            textWidth,
            textHeight};

        sdl_draw_text(renderer, currentFont, currentColor, location, menu_items[i]);
    }

    // Zobrazení nejlepšího skóre ve spodní části obrazovky
    char bestScoreText[32];
    snprintf(bestScoreText, sizeof(bestScoreText), "Best Score: %d", bestScore);

    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);

    int textWidth, textHeight;
    TTF_SizeText(smallFont, bestScoreText, &textWidth, &textHeight);

    SDL_Rect bestScoreLocation = {
        (w - textWidth) / 2,
        h - textHeight - 30, // Umístění ve spodní části s odsazením 30px od spodního okraje
        textWidth,
        textHeight};

    sdl_draw_text(renderer, smallFont, normal_color, bestScoreLocation, bestScoreText);

    // Uvolnění fontů
    TTF_CloseFont(largeFont);
    TTF_CloseFont(bigFont);
    TTF_CloseFont(smallFont);
}

int getBestScore()
{
    FILE *file = fopen("best.txt", "r+");
    int bestScore = 0;
    // file does not exist
    if (file == NULL)
    {
        return bestScore;
    }
    fscanf(file, "%d", &bestScore);
    fclose(file);
    return bestScore;
}

int menu_handle_event(SDL_Event *event, int *selected_index)
{
    if (event->type == SDL_KEYDOWN)
    {
        if (event->key.keysym.sym == SDLK_RETURN) // Enter pro výběr
        {
            if (*selected_index == 0)
            {
                // SDL_Log("%ls",selected_index);
                return MENU_START_GAME; // Start Game
            }
            else if (*selected_index == 1)
            {
                // SDL_Log("%ls",selected_index);
                return MENU_EXIT; // Exit
            }
        }
        else if (event->key.keysym.sym == SDLK_ESCAPE)
        {
            // return MENU_EXIT;// muze byt pro implementaci pozdeji nejakeho pouse
        }
        else if (event->key.keysym.sym == SDLK_UP)
        {
            if (*selected_index > 0)
            {
                (*selected_index)--; // Snížení indexu pro výběr
            }
        }
        else if (event->key.keysym.sym == SDLK_DOWN)
        {
            if (*selected_index < 1)
            {
                (*selected_index)++; // Zvýšení indexu pro výběr
            }
        }
    }
    return MENU_CONTINUE;
}