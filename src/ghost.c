#include "ghost.h"

int init_ghost(Ghost *ghost, SDL_Renderer *renderer, char *texture, Map map, char indentifier, Movements movement, Player player)
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
    ghost->homeX = x;
    ghost->homeY = y;
    ghost->renderX = x;
    ghost->renderY = y;
    ghost->id = indentifier;
    ghost->texture = IMG_LoadTexture(renderer, texture);
    if (ghost->texture == NULL)
    {
        SDL_Log("Failed to load ghost texture: %s", IMG_GetError());
        return 1;
    }
    ghost->textureBlue = IMG_LoadTexture(renderer, "../assets/GhostsEatableSpriteSheet.png");
    if (ghost->textureBlue == NULL)
    {
        SDL_Log("Failed to load ghost Blue texture: %s", IMG_GetError());
        return 1;
    }
        ghost->eatenTexture = IMG_LoadTexture(renderer, "../assets/eatenGhost.png");
    if (ghost->eatenTexture == NULL)
    {
        SDL_Log("Failed to load eaten ghost texture: %s", IMG_GetError());
        return 1;
    }
    ghost->speed = player.speed / 0.8;
    ghost->currentFrame = 0;
    ghost->frameTime = 1.08;
    ghost->timeAccumulator = 0.0;
    ghost->frameWidth = 16;  // spritesheet width
    ghost->frameHeight = 16; // spritesheet height
    ghost->totalFrames = 2;  // frames count
    ghost->state = HUNTING;
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
    if (ghost->textureBlue != NULL)
    {
        SDL_DestroyTexture(ghost->textureBlue);
        ghost->textureBlue = NULL;
    }
        if (ghost->eatenTexture != NULL)
    {
        SDL_DestroyTexture(ghost->eatenTexture);
        ghost->eatenTexture = NULL;
    }
}

int findAWayHome(Ghost *ghost, Map map) {
    Map localMap = deepCopyMap(&map);
   
    
    localMap.data[ghost->y][ghost->x] = '0'; // Teď upravujete lokální kopii
    // Directions for moving (up, down, left, right)
    int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    // Fronta pro souřadnice (použijeme paralelní pole)
    int queueX[localMap.rows * localMap.cols];
    int queueY[localMap.rows * localMap.cols];
    int front = 0, back = 0;

    // Inicializace startovní pozice
    queueX[back] = ghost->x;
    queueY[back++] = ghost->y;
    localMap.data[ghost->y][ghost->x] = '0'; // Startovní bod

    while (front < back) {
        int currentX = queueX[front];
        int currentY = queueY[front++];
        int currentWeight = localMap.data[currentY][currentX] - '0';

        for (int i = 0; i < 4; i++) {
            int nx = currentX + directions[i][0];
            int ny = currentY + directions[i][1];

            // Kontrola hranic mapy
            if (nx < 0 || ny < 0 || nx >= localMap.cols || ny >= localMap.rows) {
                continue;
            }

            // Kontrola průchodnosti buňky a zda už byla navštívena
            if (localMap.data[ny][nx] == ' ' || localMap.data[ny][nx] == '.') {
                localMap.data[ny][nx] = currentWeight + 1 + '0';
                queueX[back] = nx;
                queueY[back++] = ny;

                // Kontrola, zda jsme dosáhli cíle
                if (nx == ghost->homeX && ny == ghost->homeY) {
                    FreeMap(&localMap);
                    return currentWeight + 1;
                }
            }
        }

        showMap(localMap); // Zobrazení mapy po každém kroku
    }
            FreeMap(&localMap);
    return -1; // Cesta nebyla nalezena
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
int moveAllTheGosts(Ghost *ghost, int count, Map *map)
{
    int ret = 0;
    for (int i = 0; i < count; i++)
    {
        if ((ret = moveGhost(&ghost[i], map)) == 2)
        {
            return ret;
        }
    }
    return ret;
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
    if ((cell == 'p') && (ghost->state == HUNTING))
    {
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
        srcRect.y = 0;
        switch (ghost->state)
        {
        case HUNTING:
        {
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
            SDL_RenderCopy(renderer, ghost->texture, &srcRect, &ghostRect);
            break;
        }
        case EATEABLE:
        {
            srcRect.x = (0 * ghost->frameWidth) + (ghost->currentFrame % 2) * ghost->frameWidth;
            SDL_RenderCopy(renderer, ghost->textureBlue, &srcRect, &ghostRect);
            break;
        }
        case EATEN:
        {
            srcRect.x = (0 * ghost->frameWidth) + (ghost->currentFrame % 2) * ghost->frameWidth;
            SDL_RenderCopy(renderer, ghost->eatenTexture, &srcRect, &ghostRect);
            break;
        }
        }
    }
    else
    {
        SDL_Log("Ghost texture not set!");
    }
}
Ghost *getGhostById(Ghost *ghosts,int count, char id)
{
    for (int i = 0; i < count; i++) 
    {
        if (ghosts[i].id == id) 
        {
            return &ghosts[i]; // Vrátíme ukazatel na ducha
        }
    }
    return NULL; // Pokud žádný duch s daným ID nebyl nalezen
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
