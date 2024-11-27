#include "map.h"

void MapShow(SDL_Renderer *renderer,Map *m)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_Color color = {255, 255, 255, 255};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Texture *horizontal = IMG_LoadTexture(renderer, "../assets/horizontal.png");
    SDL_Texture *vertical = IMG_LoadTexture(renderer, "../assets/vertical.png");
    if(m==NULL)
    {
        SDL_Log("map NULL");
        return;
    }
    if (horizontal == NULL)
    {
        SDL_Log("Failed to load texture: %s", IMG_GetError());
        return;
    }
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    int wallPartSizeW = w / 100;
    int wallPartSizeH = h / 100;
    int margin = 10;
    for (int i = 0; i < m->cols; i++)
    {
        SDL_Rect wall = {wallPartSizeW,
                         wallPartSizeH,
                         margin + (i * wallPartSizeW),
                         margin};
        SDL_RenderCopy(renderer, horizontal, NULL, &wall);
    }
    SDL_RenderPresent(renderer);
    if (horizontal != NULL)
    {
        SDL_DestroyTexture(horizontal);
    }
    if (vertical != NULL)
    {
        SDL_DestroyTexture(vertical);
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
    map = initializeMap(rows, cols);
    SDL_Log("AHOJ r%d c%d ", map->rows, map->cols);
    getDatafromFile(mapFile, map);
    fclose(mapFile);
    return 0;
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
    char *line = (char *)malloc((m->cols + 1) * sizeof(char)); // +1 for '\0'
    if (line == NULL)
    {
        SDL_Log("failed to allocate line");
        return;
    }
    SDL_Log("Ahoj");
    int i = 0;
    while ((fgets(line, m->cols, f) != NULL))
    {
        if (strcmp("\n", line) != 0)
        {
            strcpy(m->data[i], line);
            SDL_Log("%s", m->data[i]);
            i++; 
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
    // Free the data array and the Map struct
    free(map->data);
    free(map);
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

    if (*cols > 0)
    {
        (*cols)++; // Include space for '\n' or '\0' during fgets
    }

    rewind(file);
    return 0;
}