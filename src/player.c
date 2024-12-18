#include "player.h"
#include "map.h"

int init_player(Player *player, SDL_Renderer *renderer, Map map)
{
    player->direction = RIGHT;
    int x, y;
    if (fingOnMap(map, 'p', &x, &y) == 1)
    {
        SDL_Log("Player not found");
        return 1;
    }
    player->x = x;
    player->y = y;
    player->renderX = x;
    player->renderY = y;
    player->speed = 10.0;

    // Načtení textury hráče
    player->texture = IMG_LoadTexture(renderer, "../assets/Pac-ManSpriteSheet.png");
    if (player->texture == NULL)
    {
        SDL_Log("Failed to load player texture: %s", IMG_GetError());
        return 1;
    }

    player->currentFrame = 0;
    player->frameTime = 0.08;
    player->timeAccumulator = 0.0;
    player->frameWidth = 32;  // spritesheet width
    player->frameHeight = 32; // spritesheet height
    player->totalFrames = 3;  // frames count
    player->score = 0;
    player->lives = 3;

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

void updatePlayer(Player *player, double deltaTime)
{
    // Update the player's animation, switching between frames of the sprite sheet
    updatePlayerAnim(player, deltaTime);

    // Smoothly interpolate the player's render position towards the logical position
    // to ensure visually fluid movement
    updatePlayerRenderPosition(player, deltaTime);
}

void updatePlayerAnim(Player *player, double deltaTime)
{
    player->timeAccumulator += deltaTime;
    if (player->timeAccumulator >= player->frameTime)
    {
        // Přepnutí na další snímek animace
        player->currentFrame = (player->currentFrame + 1) % player->totalFrames;
        player->timeAccumulator -= player->frameTime;
    }
}

void updatePlayerRenderPosition(Player *player, double deltaTime) // interpolation
{

    double step = player->speed * deltaTime;

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

    // Předpovědění nové pozice na základě směru
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
        return 0; // Žádný pohyb, pokud je směr neplatný
    }

    // Kontrola, jestli je nová pozice v rámci mapy
    if (newX < 0 || newX >= map->cols || newY < 0 || newY >= map->rows)
    {
        return 0; // Žádný pohyb mimo hranice
    }

    char cell = map->data[newY][newX];
    const char *validCells = " .o10";
    if (isCharInArray(cell, validCells) == 0)
    {
        if (cell == '1') // Pokud narazíme na teleport
        {
            int x, y;
            if (fingOnMap(*map, '0', &x, &y) == 0) // Najít teleportní cíl
            {
                map->data[player->y][player->x] = '.'; // Vymazání staré pozice hráče
                map->data[y][x - 1] = 'p';             // teleportace hrace na mape
                player->x = x - 1;
                player->y = y;
                player->renderX = x - 1;
                player->renderY = y;
                return 1; // Teleportace
            }
            return 0;
        }
        if (cell == '0') // Pokud narazíme na teleport
        {
            SDL_Log("0");
            int x, y;
            if (fingOnMap(*map, '1', &x, &y) == 0) // Najít teleportní cíl
            {

                map->data[player->y][player->x] = '.'; // Vymazání staré pozice hráče
                map->data[y][x + 1] = 'p';             // teleportace hrace na mape
                player->x = x + 1;
                player->y = y;
                player->renderX = x + 1;
                player->renderY = y;
                return 1; // Teleportace
            }
            return 0;
        }

        map->data[player->y][player->x] = '.'; // Vymazání staré pozice hráče
        map->data[newY][newX] = 'p';           // Nová pozice hráče
        player->x = newX;
        player->y = newY;

        if (cell == ' ') // Zvýšení skóre pro malou perličku
        {
            player->score += SMALLPEARL;
        }

        if (cell == 'o') // Zvýšení skóre pro velkou perličku
        {
            player->score += BIGPEARL;
            return 3;
        }
        return 1; // Pohyb úspěšný
    }

    return 0; // Pohyb zablokován překážkou
}
void renderPlayer(SDL_Renderer *renderer, Player *player, Map m)
{
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);

    int wallPartSizeW, wallPartSizeH, marginX, marginY;
    getMapMesurements(m, w, h, &wallPartSizeW, &wallPartSizeH, &marginX, &marginY);

    // Pozice na obrazovce
    SDL_Rect playerRect = {
        marginX + player->renderX * wallPartSizeW,
        marginY + player->renderY * wallPartSizeH,
        wallPartSizeW,
        wallPartSizeH};

    if (player->texture != NULL)
    {
        // Určení, který rám použít na základě směru a aktuální animace
        SDL_Rect srcRect = {
            player->currentFrame * player->frameWidth, // X pozice snímku v sprite sheetu
            0,                                         // Předpokládáme, že všechny snímky jsou v jedné řadě
            player->frameWidth,                        // Šířka jednoho snímku
            player->frameHeight                        // Výška jednoho snímku
        };

        // Rotace podle směru pohybu
        double angle = 0.0;
        SDL_RendererFlip flip = SDL_FLIP_NONE;

        // Určení rotace podle směru
        switch (player->direction)
        {
        case UP:
            angle = 270.0;
            break;
        case DOWN:
            angle = 90.0;
            break;
        case LEFT:
            angle = 180.0;
            break;
        case RIGHT:
            angle = 0.0;
            break;
        default:
            break;
        }

        // Vykreslení Pac-Mana
        SDL_RenderCopyEx(renderer, player->texture, &srcRect, &playerRect, angle, NULL, flip);
    }
    else
    {
        SDL_Log("Player texture not set!");
    }
}
void movePlayerTo(int x, int y, Player *player, Map *map)
{
    showMap(map);
    SDL_Log("player -> %d %d", player->x, player->y);
    map->data[player->y][player->x] = '.';
    map->data[y][x] = 'p';
    player->x = x;
    player->y = y;
    player->renderX = x;
    player->renderY = y;
    showMap(map);
}
void playerChangeDirection(SDL_Keycode key, Player *player, Map map)
{
    switch (key)
    {
    case SDLK_UP:
    {
        if (player->y - 1 >= 0 &&
                map.data[player->y - 1][player->x] == ' ' ||
            map.data[player->y - 1][player->x] == '.' ||
            map.data[player->y - 1][player->x] == '1' ||
            map.data[player->y - 1][player->x] == '0')
        {
            player->direction = UP;
        }
        break;
    }
    case SDLK_DOWN:
    {
        if (player->y + 1 < map.rows &&
                map.data[player->y + 1][player->x] == ' ' ||
            map.data[player->y + 1][player->x] == '.' ||
            map.data[player->y + 1][player->x] == '1' ||
            map.data[player->y + 1][player->x] == '0')
        {
            player->direction = DOWN;
        }
        break;
    }
    case SDLK_LEFT:
    {
        if (player->x - 1 >= 0 &&
                map.data[player->y][player->x - 1] == ' ' ||
            map.data[player->y][player->x - 1] == '.' ||
            map.data[player->y][player->x - 1] == '1' ||
            map.data[player->y][player->x - 1] == '0')
        {
            player->direction = LEFT;
        }
        break;
    }
    case SDLK_RIGHT:
    {
        if (player->x + 1 < map.cols &&
                map.data[player->y][player->x + 1] == ' ' ||
            map.data[player->y][player->x + 1] == '.' ||
            map.data[player->y][player->x + 1] == '1' ||
            map.data[player->y][player->x + 1] == '0')
        {
            player->direction = RIGHT;
        }
        break;
    }
    default:
        break;
    }
}

int saveBestScore(Player player)
{
    FILE *file = fopen("best.txt", "r+");

    // file did not exist
    if (file == NULL)
    {
        FILE *file = fopen("best.txt", "w+");
        fprintf(file, "%d", player.score);
        fclose(file);
        return 0;
    }
    int fileScore;
    fscanf(file, "%d", &fileScore);
    if (fileScore < player.score)
    {
        FILE *file = fopen("best.txt", "w+");
        fprintf(file, "%d", player.score);
        fclose(file);
    }
}