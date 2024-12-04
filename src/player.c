#include "universal.h"
#include "player.h"
#include "map.h"

int init_player(Player *player, SDL_Renderer *renderer, Map map)
{
    player->direction = RIGHT;
    int x, y;
    if (getPlayerLocation(map, &x, &y) == 1)
    {
        SDL_Log("Player not found");
        return 1;
    }
    player->x = x;
    player->y = y;
    player->renderX = x;
    player->renderY = y;

    // Načtení textury hráče
    player->texture = IMG_LoadTexture(renderer, "../assets/pacman.gif");
    if (player->texture == NULL)
    {
        SDL_Log("Failed to load player texture: %s", IMG_GetError());
        return 1;
    }
    return 0;
}
void free_player(Player *player)
{
    if (player->texture != NULL)
    {
        SDL_DestroyTexture(player->texture);
        player->texture = NULL;
    }
}

void updatePlayerRenderPosition(Player *player, double deltaTime, double speed)
{
    double step = speed * deltaTime;

    if (fabs(player->renderX - player->x) > step)
    {
        player->renderX += (player->x > player->renderX) ? step : -step;
    }
    else
    {
        player->renderX = player->x;
    }

    if (fabs(player->renderY - player->y) > step)
    {
        player->renderY += (player->y > player->renderY) ? step : -step;
    }
    else
    {
        player->renderY = player->y;
    }
}
int movePlayer(Player *player, Map *map)
{
    int newX = player->x;
    int newY = player->y;

    switch (player->direction)
    {
    case LEFT:
        newX--;
        break;
    case RIGHT:
        newX++;
        break;
    case UP:
        newY--;
        break;
    case DOWN:
        newY++;
        break;
    default:
        return 0; // No movement if direction is invalid
    }

    // Bound check to prevent out-of-bounds access
    if (newX < 0 || newX >= map->cols || newY < 0 || newY >= map->rows)
    {
        return 0; // No movement if outside bounds
    }

    // Check if the target cell is empty
    if (map->data[newY][newX] == ' ')
    {                                          // Correct indexing (row-major order)
        map->data[player->y][player->x] = ' '; // Clear current position
        map->data[newY][newX] = 'p';           // Move player to new position
        player->x = newX;
        player->y = newY;
        return 1; // Movement successful
    }

    return 0; // Movement blocked by obstacle
}
void renderPlayer(SDL_Renderer *renderer, Player *player, Map m)
{
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);

    int wallPartSizeW, wallPartSizeH, marginX, marginY;
    getMapMesurements(m, w, h, &wallPartSizeW, &wallPartSizeH, &marginX, &marginY);

    SDL_Rect playerRect = {
        marginX + player->renderX * wallPartSizeW,
        marginY + player->renderY * wallPartSizeH,
        wallPartSizeW,
        wallPartSizeH};

    if (player->texture != NULL)
    {
        switch (player->direction)
        {
        case UP:
            SDL_RenderCopyEx(renderer, player->texture, NULL, &playerRect, 270.0, NULL, SDL_FLIP_NONE);

            break;
        case DOWN:
            SDL_RenderCopyEx(renderer, player->texture, NULL, &playerRect, 90.0, NULL, SDL_FLIP_NONE);
            break;
        case LEFT:
            SDL_RenderCopyEx(renderer, player->texture, NULL, &playerRect, 180.0, NULL, SDL_FLIP_NONE);
            break;
        case RIGHT:
            SDL_RenderCopy(renderer, player->texture, NULL, &playerRect);

            break;
        default:
            break;
        }
    }
    else
    {
        SDL_Log("Player texture not set!");
    }
}

void changeDirection(SDL_Keycode key, Player *player, Map map)
{
    switch (key)
    {
    case SDLK_UP:
    {
        if (player->y - 1 >= 0 && map.data[player->y - 1][player->x] == ' ')
        {
            player->direction = UP;
        }
        break;
    }
    case SDLK_DOWN:
    {
        if (player->y + 1 < map.rows && map.data[player->y + 1][player->x] == ' ')
        {
            player->direction = DOWN;
        }
        break;
    }
    case SDLK_LEFT:
    {
        if (player->x - 1 >= 0 && map.data[player->y][player->x - 1] == ' ')
        {
            player->direction = LEFT;
        }
        break;
    }
    case SDLK_RIGHT:
    {
        if (player->x + 1 < map.cols && map.data[player->y][player->x + 1] == ' ')
        {
            player->direction = RIGHT;
        }
        break;
    }
    default:
        break;
    }
}

int getPlayerLocation(Map map, int *x, int *y)
{
    for (int i = 0; i < map.rows; i++)
    {
        for (int j = 0; j < map.cols; j++)
        {
            if (map.data[i][j] == 'p')
            {
                *x = j;
                *y = i;
                return 0;
            }
        }
    }
    return 1;
}
