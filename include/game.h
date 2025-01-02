#pragma once
#include "universal.h"
#include "drawing.h"
#include "ghost.h"

GameState GameTest(SDL_Renderer *renderer, double deltaTime);
void playerLostLife(Player *player, Direction pacInitDirection, int pacInitX, int pacInitY, Map *map,GameState *gameState,double *pauseTimeAccumulator);


