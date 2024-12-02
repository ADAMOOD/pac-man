#include "universal.h"
#include "player.h"

int init_player(Player *player,Map map)
{
    player->direction=RIGHT;
    int x,y;
    if(getPlayerLocation(map,&x,&y)==1)
    {
        SDL_Log("Player not found");
        return 1;
    }
    player->x=x;
    player->y=y;
    return 0;
    
}
int movePlayer(Player *player, Map *map) {
    int newX = player->x;
    int newY = player->y;

    switch (player->direction) {
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
        return 0; // No movement
    }

    // Bound Check
    if (newX < 0 || newX > map->cols || newY < 0 || newY > map->rows) {
        return 0; // Nothing if outside
    }
    else if(map->data[newX][newY]==' ')
    {
        map->data[player->x][player->y]==' ';
        map->data[newX][newY]=='p';
        player->x=newX;
        player->y=newY;
        return 1;
    }
    return 0; 
}

int getPlayerLocation(Map map,int *x,int *y)
{
    for(int i=0;i<map.rows;i++)
    {
        for(int j=0;j<map.cols;j++)
        {
             if(map.data[i][j]=='p')
             {
                *x=i;
                *y=j;
                return 0;
             }
        }
    }
    return 1;
}
