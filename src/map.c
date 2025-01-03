#include "map.h"
#include "drawing.h"

int renderMap(SDL_Renderer *renderer, Map m)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Load textures for horizontal and vertical walls
    SDL_Texture *horizontal = IMG_LoadTexture(renderer, "../assets/horizontal.png");
    SDL_Texture *corner = IMG_LoadTexture(renderer, "../assets/leftUpCorner.png");
    SDL_Texture *topPipe = IMG_LoadTexture(renderer, "../assets/toppipe.png");
    SDL_Texture *endDown = IMG_LoadTexture(renderer, "../assets/endDown.png");

    if (mapIsInvalid(m))
    {
        SDL_Log("invalid map");
        return 1;
    }

    if (horizontal == NULL || corner == NULL || topPipe == NULL || endDown == NULL)
    {
        SDL_Log("Failed to load texture: %s", IMG_GetError());
        return 1;
    }

    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);

    int wallPartSizeW, wallPartSizeH, marginX, marginY;
    getMapMesurements(m, w, h, &wallPartSizeW, &wallPartSizeH, &marginX, &marginY);

    SDL_Rect wall;

    for (int r = 0; r < m.rows; r++)
    {
        for (int c = 0; c < m.cols; c++)
        {
            wall = (SDL_Rect){
                marginX + (c * wallPartSizeW),
                marginY + (r * wallPartSizeH),
                wallPartSizeW,
                wallPartSizeH};
            switch (m.data[r][c])
            {
            case '-':
            {
                if (m.data[r][c + 1] == ' ' || m.data[r][c + 1] == 'p' || m.data[r][c + 1] == '.')
                {
                    SDL_RenderCopyEx(renderer, endDown, NULL, &wall, 270.0, NULL, SDL_FLIP_NONE);
                }
                else if (m.data[r][c - 1] == ' ' || m.data[r][c - 1] == 'p' || m.data[r][c - 1] == '.')
                {
                    SDL_RenderCopyEx(renderer, endDown, NULL, &wall, 90.0, NULL, SDL_FLIP_NONE);
                }
                else
                {
                    SDL_RenderCopy(renderer, horizontal, NULL, &wall);
                }
                break;
            }
            case '|':
            {
                const char *validCells = " .o10pc";
                if (isCharInArray(m.data[r + 1][c],validCells)==0)
                {
                    SDL_RenderCopy(renderer, endDown, NULL, &wall);
                }
                else if (isCharInArray(m.data[r - 1][c],validCells)==0)
                {
                    SDL_RenderCopyEx(renderer, endDown, NULL, &wall, 180.0, NULL, SDL_FLIP_NONE);
                }
                else
                {
                    SDL_RenderCopyEx(renderer, horizontal, NULL, &wall, 90.0, NULL, SDL_FLIP_NONE);
                }
                break;
            }
            case '/':
            {
                // top left (default)
                if ((r + 1) < m.rows && m.data[r + 1][c] == '|' &&
                    (c + 1) < m.cols && m.data[r][c + 1] == '-')
                {
                    SDL_RenderCopy(renderer, corner, NULL, &wall);
                }
                // right top
                else if ((r + 1) < m.rows && m.data[r + 1][c] == '|' &&
                         (c - 1) >= 0 && m.data[r][c - 1] == '-')
                {
                    SDL_RenderCopyEx(renderer, corner, NULL, &wall, 90.0, NULL, SDL_FLIP_NONE);
                }
                // left lower
                else if ((r - 1) >= 0 && m.data[r - 1][c] == '|' &&
                         (c + 1) < m.cols && m.data[r][c + 1] == '-')
                {
                    SDL_RenderCopyEx(renderer, corner, NULL, &wall, 270.0, NULL, SDL_FLIP_NONE);
                }
                // right lower
                else if ((r - 1) >= 0 && m.data[r - 1][c] == '|' &&
                         (c - 1) >= 0 && m.data[r][c - 1] == '-')
                {
                    SDL_RenderCopyEx(renderer, corner, NULL, &wall, 180.0, NULL, SDL_FLIP_NONE);
                }
                else if ((r - 1) >= 0 && m.data[r - 1][c] == '/' &&
                         (c + 1) < m.cols && m.data[r][c + 1] == '-') // case where there are two corners on top
                {
                    SDL_RenderCopyEx(renderer, corner, NULL, &wall, 270.0, NULL, SDL_FLIP_NONE);
                }
                break;
            }
            case 'T':
                if ((r + 1) < m.rows && m.data[r + 1][c] == '|' && m.data[r][c + 1] == '-' && m.data[r][c - 1] == '-')
                {
                    SDL_RenderCopy(renderer, topPipe, NULL, &wall);
                }
                else if ((r - 1) > 0 && (r + 1) < m.rows && m.data[r + 1][c] == '|' && m.data[r - 1][c] == '|' && m.data[r][c + 1] == '-')
                {
                    SDL_RenderCopyEx(renderer, topPipe, NULL, &wall, 270.0, NULL, SDL_FLIP_NONE);
                }
                else
                {
                    SDL_RenderCopyEx(renderer, topPipe, NULL, &wall, 180.0, NULL, SDL_FLIP_NONE);
                }
                break;
            case ' ':
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 

                int radius = wallPartSizeW < wallPartSizeH ? wallPartSizeW / 8 : wallPartSizeH / 8; // Adjust radius
                int centerX = marginX + (c * wallPartSizeW) + (wallPartSizeW / 2);
                int centerY = marginY + (r * wallPartSizeH) + (wallPartSizeH / 2);

                RenderFilledCircle(renderer, centerX, centerY, radius);
                break;
            }
            case 'o':
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 

                int radius = wallPartSizeW < wallPartSizeH ? wallPartSizeW / 3 : wallPartSizeH / 3; // Adjust radius
                int centerX = marginX + (c * wallPartSizeW) + (wallPartSizeW / 2);
                int centerY = marginY + (r * wallPartSizeH) + (wallPartSizeH / 2);

                RenderFilledCircle(renderer, centerX, centerY, radius);
                break;
            }

            break;
            }
        }
    }
    // Clean up textures after rendering
    if (horizontal != NULL)
    {
        SDL_DestroyTexture(horizontal);
    }
    if (corner != NULL)
    {
        SDL_DestroyTexture(corner);
    }
    if (topPipe != NULL)
    {
        SDL_DestroyTexture(topPipe);
    }
    if (endDown != NULL)
    {
        SDL_DestroyTexture(endDown);
    }

    return 0;
}
int checkForPearlsLeft(Map map)
{
    for(int i=0;i<map.rows;i++)
    {
        for(int n=0;n<map.cols;n++)
        {
            if(map.data[i][n]==' '||map.data[i][n]=='o')
            {
                return 1;
            }
        }
    }
    return 0;
}

void getMapMesurements(Map map, int w, int h, int *wallPartSizeW, int *wallPartSizeH, int *marginX, int *marginY)
{
    // Propočítání velikostí dílů podle poměru obrazovky a mapy
    int partWidth = w / map.cols;
    int partHeight = h / map.rows;

    int minPartSize = (partWidth < partHeight) ? partWidth : partHeight;

    *wallPartSizeW = minPartSize;
    *wallPartSizeH = minPartSize;

    // margins of the map for its center pos
    *marginX = (w - (map.cols * minPartSize)) / 2;
    *marginY = (h - (map.rows * minPartSize)) / 2;
}

void printMapInDetail(Map m)
{
    if (mapIsInvalid(m))
    {
        SDL_Log("Map is invalid");
        return;
    }
    for (int r = 0; r < m.rows; r++)
    {
        for (int c = 0; c < m.cols; c++)
        {
            SDL_Log(" %d [%c] ", c, m.data[r][c]);
        }
        SDL_Log("\n");
    }
}

int GetMapFile(Map *map)
{
    FILE *mapFile;
    if ((mapFile = fopen(MAP_PATH, "r")) == NULL)
    {
        SDL_Log("Failed to open file");
        return -1;
    }
    int rows, cols;
    if (getFileRowsAndCols(mapFile, &cols, &rows) == -1)
    {
        SDL_Log("Failed to analyze Map file");
        fclose(mapFile);
        return -1;
    }
    *map = allocateMap(rows, cols);
    getDatafromFile(mapFile, map);
    fclose(mapFile);
    fingOnMap(*map,'0',&map->tele0[0],&map->tele0[1]);
    fingOnMap(*map,'1',&map->tele1[0],&map->tele1[1]);
    return 0;
}

Map deepCopyMap(const Map *original) {
    Map copy;
    copy.rows = original->rows;
    copy.cols = original->cols;

    copy.data = malloc(copy.rows * sizeof(char *));
    for (int i = 0; i < copy.rows; i++) {
        copy.data[i] = malloc(copy.cols * sizeof(char));
        memcpy(copy.data[i], original->data[i], copy.cols);
    }
    return copy;
}

void showMap(Map map) {

    if (mapIsInvalid(map))
    {
        SDL_Log("Map is invalid");
        return;
    }

    int totalSize = (map.rows * map.cols) + map.rows + 1; // + map->rows for \n a +1 for \0

    char *result = (char *)malloc(totalSize * sizeof(char));
    if (result == NULL) {
        SDL_Log("Failed to allocate memory for map string");
        return;
    }

    int pos = 0; 
    for (int i = 0; i < map.rows; i++) {
        for (int j = 0; j < map.cols; j++) {
            if (map.data[i][j] == '\0') {
                SDL_Log("Unexpected null character in map data at row %d, col %d", i, j);
                result[pos++] = '?'; // replacement of the invalid char
            } else {
                result[pos++] = map.data[i][j];
            }
        }
        result[pos++] = '\n';
    }
    result[pos] = '\0';

    SDL_Log("Map (%dx%d):\n%s", map.rows, map.cols, result);

    free(result);
}

int mapIsInvalid(Map map)
{
    return map.data == NULL || map.cols == 0 || map.rows == 0;
}
void getDatafromFile(FILE *f, Map *m)
{
    if (f == NULL)
    {
        SDL_Log("NULL file");
        return;
    }
    if (m == NULL)
    {
        SDL_Log("NULL map");
        return;
    }

    int r = 0, c = 0;
    char ch;
    while ((ch = fgetc(f)) != EOF)
    {
        if (ch != '\n')
        {
            if (r < m->rows && c < m->cols)
            {
                m->data[r][c] = ch;
                c++; // next column
            }
        }
        if (ch == '\n') // if new line
        {
            r++;   // increese rows
            c = 0; // Reset columns
        }
    }

}

Map allocateMap(int rows, int cols)
{
    Map map;
    map.rows = rows;
    map.cols = cols;

    // Allocate memory for the data array (array of char pointers)
    map.data = (char **)malloc(rows * sizeof(char *));
    if (map.data == NULL)
    {
        SDL_Log("Failed to allocate memory for data");
        free(map.data); // Free previously allocated memory
        return map;
    }
    // Allocate memory for each row in the data array
    for (int i = 0; i < rows; i++)
    {
        map.data[i] = (char *)malloc(cols * sizeof(char));
        if (map.data[i] == NULL)
        {
            SDL_Log("Failed to allocate memory for a row");
            for (int j = 0; j < i; j++)
            {
                free(map.data[j]);
            }
            free(map.data);
            SDL_Log("failed to allocate %d. row",i);
        }
    }
    return map;
}
void FreeMap(Map *map)
{
    if (map == NULL)
        return;

    // Free each row in the data array
    for (int i = 0; i < map->rows; i++)
    {
        free(map->data[i]);
    }

    // Free the data array
    free(map->data);

    // Reset map fields
    map->data = NULL;
    map->rows = 0;
    map->cols = 0;
    map = NULL;
}
int getFileRowsAndCols(FILE *file, int *cols, int *rows)
{
    if (file == NULL)
    {
        return -1;
    }
    char c;
    *cols = 0;
    *rows = 0;
    int isFirstRow = 1;

    while ((c = fgetc(file)) != EOF)
    {
        if (isFirstRow && c != '\n')
        {
            (*cols)++;
        }

        if (c == '\n')
        {
            (*rows)++;
            isFirstRow = 0;
        }
    }
    // Handle case where last row does not end with a newline
    if (c != '\n' && *cols > 0)
    {
        (*rows)++;
    }
    rewind(file);
    return 0;
}
int fingOnMap(Map map,char c, int *x, int *y)
{
    for (int i = 0; i < map.rows; i++)
    {
        for (int j = 0; j < map.cols; j++)
        {
            if (map.data[i][j] == c)
            {
                *x = j;
                *y = i;
                return 0;
            }
        }
    }
    return 1;
}