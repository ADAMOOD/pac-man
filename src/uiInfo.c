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
    if (fontSize < 12)
        fontSize = 12; // Minimal size
    if (fontSize > 50)
        fontSize = 50; // Maximal size
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

    // Ensure minimal space below the map
    int minMarginBottom = h / 20;

    SDL_Rect textLocation = {
        marginX + (wallPartSizeW * map.cols - textWidth) / 2, // Centered horizontally in map
        marginY + wallPartSizeH * map.rows + minMarginBottom, // Below the map
        textWidth,
        textHeight};
    if (textLocation.y + textLocation.h > h)
    {
        textLocation.y = h - textLocation.h - minMarginBottom;
    }

    SDL_Color white = {255, 255, 255, 255};
    sdl_draw_text(renderer, font, white, textLocation, scoreText);

    // Render player lives as sprites
    if (player.texture != NULL)
    {
        SDL_Rect srcRect = {
            0,                     // First frame of the sprite sheet
            0,
            player.frameWidth,     // Width of one frame
            player.frameHeight};   // Height of one frame

        SDL_Rect destRect = {
            textLocation.x + textWidth + fontSize, // Right after the score text
            textLocation.y,
            fontSize,                             // Scale the sprite to the font size
            fontSize};

        for (int i = 0; i < player.lives; i++)
        {
            SDL_RenderCopy(renderer, player.texture, &srcRect, &destRect);

            // Move to the right for the next sprite
            destRect.x += fontSize + 5; // Add small padding between sprites
        }
    }

    TTF_CloseFont(font);
}



