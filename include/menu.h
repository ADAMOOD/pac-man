#pragma once

#include "universal.h"

// Konstanty pro stav menu
#define MENU_CONTINUE 0
#define MENU_START_GAME 1
#define MENU_EXIT 2

// Funkce pro vykreslení menu
void menu_render(SDL_Renderer *renderer, int selected_index);

// Funkce pro zpracování událostí v menu
int menu_handle_event(SDL_Event *event, int *selected_index);

