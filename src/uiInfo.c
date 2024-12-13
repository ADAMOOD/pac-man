#include "player.h"
#include "map.h"
#include "drawing.h"

void renderUI(Player player, Map map, SDL_Renderer *renderer)
{
    char scoreText[50];
    snprintf(scoreText, sizeof(scoreText), "%d", player.score); 

    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);

    // Dynamic size of text depending on window width
    int fontSize = w / 25;
    if (fontSize < 12) fontSize = 12; // minimal size
    if (fontSize > 50) fontSize = 50; // maximal size

    // Načtení fontu s danou velikostí
    TTF_Font *font = TTF_OpenFont("pixelated.ttf", fontSize);
    if (!font)
    {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        return;
    }

    int textWidth, textHeight;
    TTF_SizeText(font, scoreText, &textWidth, &textHeight); 

    int wallPartSizeW, wallPartSizeH, marginX, marginY;
    getMapMesurements(map, w, h, &wallPartSizeW, &wallPartSizeH, &marginX, &marginY);

    // Zajištění minimálního volného prostoru pod mapou
    int minMarginBottom = h / 20; // Minimální spodní mezera (5 % výšky obrazovky)

    // Výpočet pozice textu
    SDL_Rect location = {
        marginX + (wallPartSizeW * map.cols - textWidth) / 2,  // X: zarovnání na střed mapy
        marginY + wallPartSizeH * map.rows + minMarginBottom, // Y: pod mapou s mezerou
        textWidth,                                            // Šířka textu
        textHeight                                            // Výška textu
    };

    // Kontrola, jestli text nepřesahuje obrazovku
    if (location.y + location.h > h)
    {
        location.y = h - location.h - minMarginBottom; // Posunout text výše, aby byl viditelný
    }

    // Nastavení barvy textu (bílá)
    SDL_Color white = {255, 255, 255, 255};

    // Vykreslení textu (např. skóre)
    sdl_draw_text(renderer, font, white, location, scoreText);


    TTF_CloseFont(font);
}

