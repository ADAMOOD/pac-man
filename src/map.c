#include "map.h"

int MapShow(SDL_Renderer *renderer, Map m)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Load textures for horizontal and vertical walls
    SDL_Texture *horizontal = IMG_LoadTexture(renderer, "../assets/horizontal.png");
    SDL_Texture *corner = IMG_LoadTexture(renderer, "../assets/leftUpCorner.png");
    SDL_Texture *topPipe = IMG_LoadTexture(renderer, "../assets/toppipe.png");
    SDL_Texture *endDown = IMG_LoadTexture(renderer, "../assets/endDown.png");
    SDL_Texture *pacman = IMG_LoadTexture(renderer, "../assets/pacman.gif");

    if (mapIsInvalid(m))
    {
        SDL_Log("invalid map");
        return 1;
    }

    if (horizontal == NULL || corner == NULL || topPipe == NULL || endDown == NULL || pacman == NULL)
    {
        SDL_Log("Failed to load texture: %s", IMG_GetError());
        return 1;
    }

    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    
    int wallPartSizeW = w / 30; // Wall part size based on window width
    int wallPartSizeH = h / 30; // Wall part size based on window height
    
    // Calculate the offset to center the map
    int marginX = (w - m.cols * wallPartSizeW) / 2;
    int marginY = (h - m.rows * wallPartSizeH) / 2;

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
                if (m.data[r][c + 1] == ' ')
                {
                    SDL_RenderCopyEx(renderer, endDown, NULL, &wall, 270.0, NULL, SDL_FLIP_NONE);
                }
                else if (m.data[r][c - 1] == ' ')
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
                if (m.data[r + 1][c] == ' ')
                {
                    SDL_RenderCopy(renderer, endDown, NULL, &wall);
                }
                else if (m.data[r - 1][c] == ' ')
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
            case 'p':
                SDL_RenderCopy(renderer, pacman, NULL, &wall);
                break;
            default:
                break;
            }
        }
    }

    SDL_RenderPresent(renderer); // Update the screen with rendered walls

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
    if (pacman != NULL)
    {
        SDL_DestroyTexture(pacman);
    }

    return 0;
}

void printMapInDetail(Map m)
{
    if (mapIsInvalid(m))
    {
        SDL_Log("Map is invalid");
        return;
    }

    // Iterate through the map rows and columns to print the map.
    for (int r = 0; r < m.rows; r++)
    {
        for (int c = 0; c < m.cols; c++)
        {
            SDL_Log(" %d [%c] ", c, m.data[r][c]); // Print each character in the map
        }
        SDL_Log("\n"); // Newline after each row
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
    *map = *initializeMap(rows, cols);
    getDatafromFile(mapFile, map);
    fclose(mapFile);
    return 0;
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

Map *initializeMap(int rows, int cols)
{
    // Allocate memory for the Map struct
    Map *map = (Map *)malloc(sizeof(Map));
    if (map == NULL)
    {
        SDL_Log("Failed to allocate memory for Map");
        return NULL;
    }
    map->rows = rows;
    map->cols = cols;
    // Allocate memory for the data array (array of char pointers)
    map->data = (char **)malloc(rows * sizeof(char *));
    if (map->data == NULL)
    {
        SDL_Log("Failed to allocate memory for data");
        free(map); // Free previously allocated memory
        return NULL;
    }

    // Allocate memory for each row in the data array
    for (int i = 0; i < rows; i++)
    {
        map->data[i] = (char *)malloc(cols * sizeof(char));
        if (map->data[i] == NULL)
        {
            SDL_Log("Failed to allocate memory for a row");
            for (int j = 0; j < i; j++)
            {
                free(map->data[j]);
            }
            free(map->data);
            free(map);
            return NULL;
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