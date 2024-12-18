#include "ghost.h"

int init_ghost(Ghost *ghost, SDL_Renderer *renderer,char* texture, Map map, char indentifier, Movements movement, Player player)
{
    ghost->direction = UP;
    int x, y;
    if (fingOnMap(map, indentifier, &x, &y) == 1)
    {
        SDL_Log("ghost %c not found", indentifier);
        return 1;
    }
    ghost->lastCell = '.';
    ghost->x = x;
    ghost->y = y;
    ghost->renderX = x;
    ghost->renderY = y;
    ghost->id = indentifier;
    ghost->texture = IMG_LoadTexture(renderer, texture);
    if (ghost->texture == NULL)
    {
        SDL_Log("Failed to load ghost texture: %s", IMG_GetError());
        return 1;
    }
    ghost->speed = player.speed / 0.8;
    ghost->currentFrame = 0;
    ghost->frameTime = 1.08;
    ghost->timeAccumulator = 0.0;
    ghost->frameWidth = 16;  // spritesheet width
    ghost->frameHeight = 16; // spritesheet height
    ghost->totalFrames = 2;  // frames count
    ghost->state = 1;
    ghost->movement = movement;
    return 0;
}
void free_ghost(Ghost *ghost)
{
    if (ghost->texture != NULL)
    {
        SDL_DestroyTexture(ghost->texture);
        ghost->texture = NULL;
    }
}

void updateGhost(Ghost *ghost, double deltaTime)
{
    // Update the ghost's animation, switching between frames of the sprite sheet
    updateGhostAnim(ghost, deltaTime);

    // Smoothly interpolate the ghost's render position towards the logical position
    // to ensure visually fluid movement
    updateGhostRenderPosition(ghost, deltaTime);
}

void updateGhostAnim(Ghost *ghost, double deltaTime)
{
    ghost->timeAccumulator += deltaTime;
    if (ghost->timeAccumulator >= ghost->frameTime)
    {
        // Přepnutí na další snímek animace
        ghost->currentFrame = (ghost->currentFrame + 1) % ghost->totalFrames;
        ghost->timeAccumulator -= ghost->frameTime;
    }
}

void updateGhostRenderPosition(Ghost *ghost, double deltaTime) // interpolation
{

    double step = ghost->speed * deltaTime;

    if (fabs(ghost->renderX - ghost->x) > step)
    {
        ghost->renderX += (ghost->x > ghost->renderX) ? step : -step;
    }
    else
    {
        ghost->renderX = ghost->x;
    }

    if (fabs(ghost->renderY - ghost->y) > step)
    {
        ghost->renderY += (ghost->y > ghost->renderY) ? step : -step;
    }
    else
    {
        ghost->renderY = ghost->y;
    }
}
int moveGhost(Ghost *ghost, Map *map)
{
    int newX = ghost->x;
    int newY = ghost->y;

    // Předpovědění nové pozice na základě směru
    switch (ghost->direction)
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
                map->data[ghost->y][ghost->x] = '.'; // Vymazání staré pozice hráče
                map->data[y][x - 1] = ghost->id;     // teleportace hrace na mape
                ghost->x = x - 1;
                ghost->y = y;
                ghost->renderX = x - 1;
                ghost->renderY = y;
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
                map->data[ghost->y][ghost->x] = '.'; // Vymazání staré pozice hráče
                map->data[y][x + 1] = ghost->id;     // teleportace hrace na mape
                ghost->x = x + 1;
                ghost->y = y;
                ghost->renderX = x + 1;
                ghost->renderY = y;
                return 1; // Teleportace
            }
            return 0;
        }

        map->data[ghost->y][ghost->x] = ghost->lastCell; // Vymazání staré pozice hráče
        ghost->lastCell = cell;
        map->data[newY][newX] = ghost->id; // Nová pozice hráče
        ghost->x = newX;
        ghost->y = newY;
        ghost->isMooving = 1;
        return 1; // Pohyb úspěšný
    }
    if (cell == 'p')
    {
        SDL_Log("Life lost");
        return 2;
    }
    ghost->isMooving = 0;
    setRandomDirection(ghost);
    return 0; // Pohyb zablokován překážkou
}
void renderGhost(SDL_Renderer *renderer, Ghost *ghost, Map m)
{
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);

    int wallPartSizeW, wallPartSizeH, marginX, marginY;
    getMapMesurements(m, w, h, &wallPartSizeW, &wallPartSizeH, &marginX, &marginY);

    // Pozice na obrazovce
    SDL_Rect ghostRect = {
        marginX + ghost->renderX * wallPartSizeW,
        marginY + ghost->renderY * wallPartSizeH,
        wallPartSizeW,
        wallPartSizeH};

    if (ghost->texture != NULL)
    {
        // Určení, který rám použít na základě směru
        SDL_Rect srcRect = {0, 0, ghost->frameWidth, ghost->frameHeight};

        // Změna snímků na základě směru pohybu
switch (ghost->direction)
{
    case UP:
        srcRect.x = (4 * ghost->frameWidth) + (ghost->currentFrame % 2) * ghost->frameWidth;
        break;
    case DOWN:
        srcRect.x = (6 * ghost->frameWidth) + (ghost->currentFrame % 2) * ghost->frameWidth;
        break;
    case LEFT:
        srcRect.x = (2 * ghost->frameWidth) + (ghost->currentFrame % 2) * ghost->frameWidth;
        break;
    case RIGHT:
        srcRect.x = (0 * ghost->frameWidth) + (ghost->currentFrame % 2) * ghost->frameWidth;
        break;
    default:
        srcRect.x = 0; // Výchozí pozice
        break;
}


        // Aktualizace snímku na základě aktuální animace
        srcRect.y = 0; // Assuming all frames are in one row of the sprite sheet

        // Vykreslení ducha s aktuálním snímkem
        SDL_RenderCopy(renderer, ghost->texture, &srcRect, &ghostRect);
    }
    else
    {
        SDL_Log("Ghost texture not set!");
    }
}

void setRandomDirection(Ghost *ghost)
{
    // Seed the random number generator (you can call this once in the main program)
    // srand(time(NULL)); // Uncomment this in your main function or during initialization

    // Generate a random number between 0 and 3 to pick a direction
    int randomDirection = rand() % 4;

    // Set the ghost's direction based on the random number
    switch (randomDirection)
    {
    case 0:
        ghost->direction = UP;
        break;
    case 1:
        ghost->direction = DOWN;
        break;
    case 2:
        ghost->direction = LEFT;
        break;
    case 3:
        ghost->direction = RIGHT;
        break;
    default:
        ghost->direction = UP; // Default to UP if something goes wrong
        break;
    }
}
