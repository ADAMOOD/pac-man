#include "universal.h"
#include "player.h"

int init_player(Player *player, Map map)
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
    return 0;
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
                *x = i;
                *y = j;
                return 0;
            }
        }
    }
    return 1;
}
