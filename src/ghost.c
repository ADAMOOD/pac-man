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
    ghost->speed = player.speed * 0.9;
    ghost->currentFrame = 0;
    ghost->frameTime = 1.08;
    ghost->timeAccumulator = 0.0;
    ghost->frameWidth = 16;  // spritesheet width
    ghost->frameHeight = 16; // spritesheet height
    ghost->totalFrames = 2;  // frames count
    ghost->state = HUNTING;
    ghost->movement = movement;
    ghost->stepsToHome = 0;
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
    freeWayHome(ghost);
}

int findAWayHome(Ghost *ghost, Map map)
{
    Map localMap = deepCopyMap(&map);

    localMap.data[ghost->y][ghost->x] = '3';

    int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    // Queue for coordinates (using parallel arrays)
    int queueX[localMap.rows * localMap.cols];
    int queueY[localMap.rows * localMap.cols];
    int front = 0, back = 0;

    // Initialize the starting position
    queueX[back] = ghost->x;
    queueY[back++] = ghost->y;
    localMap.data[ghost->y][ghost->x] = '3'; // Starting point

    while (front < back)
    {
        int currentX = queueX[front];
        int currentY = queueY[front++];
        int currentWeight = localMap.data[currentY][currentX] - '3';

        for (int i = 0; i < 4; i++)
        {
            int nx = currentX + directions[i][0];
            int ny = currentY + directions[i][1];

            // Check map boundaries
            if (nx < 0 || ny < 0 || nx >= localMap.cols || ny >= localMap.rows)
            {
                continue;
            }
            // Check if the cell is passable and not visited
            if (localMap.data[ny][nx] == ' ' || localMap.data[ny][nx] == '.' || localMap.data[ny][nx] == 'o')
            {
                localMap.data[ny][nx] = currentWeight + 1 + '3';
                queueX[back] = nx;
                queueY[back++] = ny;

                // Check if we have reached the target
                if (nx == ghost->homeX && ny == ghost->homeY)
                {
                    // showMap(localMap);
                    writeStepsToGoHome(ghost, localMap);
                    FreeMap(&localMap);
                    return 0;
                }
            }
        }
    }
    FreeMap(&localMap);
    return -1; // Path not found
}

void freeWayHome(Ghost *ghost)
{
    ghost->stepsToHome=0;
    if (ghost->wayHome != NULL)
    {
        for (int i = 0; i < ghost->stepsToHome + 1; i++)
        {
            free(ghost->wayHome[i]);
        }
        free(ghost->wayHome);
        ghost->wayHome = NULL;
    }
}
int writeStepsToGoHome(Ghost *ghost, Map map)
{
    int stepsHomeCount = (int)(map.data[ghost->homeY][ghost->homeX] - '3');
    /*SDL_Log("Ghost %c home is on [%d:%d] with char '%c', which is step %d.",
            ghost->id, ghost->homeY, ghost->homeX, map.data[ghost->homeY][ghost->homeX], stepsHomeCount);*/
    ghost->stepsToHome = stepsHomeCount;
    // Alokace paměti pro pole ukazatelů (přidáváme 1 pro cílový bod '3')
    ghost->wayHome = (int **)malloc((stepsHomeCount + 1) * sizeof(int *));
    if (ghost->wayHome == NULL)
    {
        SDL_Log("Failed to allocate memory for wayHome");
        return -1;
    }

    // Alokace paměti pro každou dvojici [x, y]
    for (int i = 0; i < stepsHomeCount + 1; i++) // Alokujeme pro stepsHomeCount + 1 (poslední pozici)
    {
        ghost->wayHome[i] = (int *)malloc(2 * sizeof(int)); // Pro každý krok x, y
        if (ghost->wayHome[i] == NULL)
        {
            SDL_Log("Failed to allocate memory for wayHome[%d]", i);

            // Uvolnění již alokované paměti v případě chyby
            for (int j = 0; j < i; j++)
            {
                free(ghost->wayHome[j]);
            }
            free(ghost->wayHome);
            ghost->wayHome = NULL;
            return -1;
        }
    }

    // Zapisování souřadnic krok po kroku
    int currentX = ghost->homeX;
    int currentY = ghost->homeY;
    int step = 0;

    // Definice směrové matice pro pohyby (nahoru, dolů, vlevo, vpravo)
    int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    // Procházení mapy od domácí pozice až k cíli
    while (map.data[currentY][currentX] != '3' && step < stepsHomeCount) // Zajištění, že nezajdeme mimo
    {
        // Uložení aktuálních souřadnic do pole
        ghost->wayHome[step][0] = currentX;
        ghost->wayHome[step][1] = currentY;
        step++;

        // Zkontroluj všechny sousední buňky a najdi tu s menší hodnotou
        char currentChar = map.data[currentY][currentX];
        char minValue = currentChar - 1; // Hledáme hodnotu o 1 menší
        int nextX = currentX;
        int nextY = currentY;

        for (int i = 0; i < 4; i++) // Možné směry (nahoru, dolů, vlevo, vpravo)
        {
            int nx = currentX + directions[i][0];
            int ny = currentY + directions[i][1];

            // Kontrola hranic mapy
            if (nx >= 0 && ny >= 0 && nx < map.cols && ny < map.rows)
            {
                // Zkontroluj, zda je hodnota buňky o 1 menší než aktuální
                if (map.data[ny][nx] == minValue)
                {
                    nextX = nx;
                    nextY = ny;
                    break; // Pokud jsme našli správnou hodnotu, zastavíme hledání
                }
            }
        }

        // Pokud jsme našli platnou cestu, posuneme se na novou pozici
        if (nextX != currentX || nextY != currentY)
        {
            currentX = nextX;
            currentY = nextY;
        }
        else
        {
            SDL_Log("No valid next step found from [%d, %d]", currentX, currentY);
            break; // Pokud není platná cesta, přerušíme cyklus
        }
    }

    // Uložení poslední pozice (cílový bod '3')
    ghost->wayHome[step][0] = currentX;
    ghost->wayHome[step][1] = currentY;
    step++;
    /*
        for (int i = 0; i < step; i++)
        {
            SDL_Log("Step %d: [%d, %d] char '%c'", i, ghost->wayHome[i][0], ghost->wayHome[i][1], map.data[ghost->wayHome[i][1]][ghost->wayHome[i][0]]);
        }
    */
    return step;
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
    // Handle ghost's homecoming when eaten
    if (ghost->state == EATEN && ghost->stepsToHome != 0)
    {
        ghost->lastCell = '.';
        newX = ghost->wayHome[ghost->stepsToHome][0];
        newY = ghost->wayHome[ghost->stepsToHome][1];
        char cell = map->data[newY][newX];
        map->data[ghost->y][ghost->x] = ghost->lastCell; // Clear old position
        ghost->lastCell = cell;
        map->data[newY][newX] = ghost->id; // Set new position
        ghost->x = newX;
        ghost->y = newY;
        ghost->isMooving = 1;

        // Free memory for the current step
        free(ghost->wayHome[ghost->stepsToHome]);
        ghost->stepsToHome--;

        // If no more steps, free the wayHome array and set state to HUNTING
        if (ghost->stepsToHome == 0)
        {
            freeWayHome(ghost);
            ghost->state = HUNTING; // Corrected to assignment
        }
        return 1; // Successful movement
    }
    if (ghost->state == EATEN && ghost->x == ghost->homeX && ghost->y == ghost->homeY)
    {
        freeWayHome(ghost);
        ghost->state = HUNTING; // Corrected to assignment
    }
    if (ghost->state == EATEN && ghost->stepsToHome == 0)
    {
        findAWayHome(ghost,*map);
    }
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
       /* if((rand() % 4)==1)
        {
            setRandomDirection(ghost,*map);
        }*/
        return 1; // Pohyb úspěšný
    }
    if ((cell == 'p') && (ghost->state == HUNTING))
    {
        return 2;
    }
    ghost->isMooving = 0;
    setRandomDirection(ghost, *map);
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
Ghost *getGhostById(Ghost *ghosts, int count, char id)
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
void setRandomDirection(Ghost *ghost, Map map)
{
    int directions[4] = {UP, DOWN, LEFT, RIGHT};
    int shuffledIndices[4] = {0, 1, 2, 3};

    // Shuffle the directions to try them in a random order
    for (int i = 3; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = shuffledIndices[i];
        shuffledIndices[i] = shuffledIndices[j];
        shuffledIndices[j] = temp;
    }

    // Try each direction in the shuffled order
    for (int i = 0; i < 4; i++)
    {
        int dir = directions[shuffledIndices[i]];
        int nextX = ghost->x;
        int nextY = ghost->y;

        // Calculate the new position based on the direction
        switch (dir)
        {
        case UP:
            nextY--;
            break;
        case DOWN:
            nextY++;
            break;
        case LEFT:
            nextX--;
            break;
        case RIGHT:
            nextX++;
            break;
        }

        // Check if the new position is within the map boundaries and passable
        if (nextX >= 0 && nextY >= 0 && nextX < map.cols && nextY < map.rows)
        {
            char nextCell = map.data[nextY][nextX];
            if (nextCell == ' ' || nextCell == '.' || nextCell == 'o') // Passable cells
            {
                ghost->direction = dir;
                return;
            }
        }
    }

    // If no valid direction is found, default to no movement
    ghost->direction = UP; // Default direction or some fallback behavior
}

