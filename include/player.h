#pragma once

#include <SDL2/SDL.h>
#include "map.h"

typedef struct {
    int x, y;                    // Logical position of the player on the map (x, y coordinates)
    double renderX, renderY;     // Rendered (interpolated) position on the screen (smooth movement)
    Direction direction;         // Direction in which the player is moving (up, down, left, right)
    double speed;                // Speed of the player's movement
    SDL_Texture *texture;        // Texture for the entire player character (sprite sheet)
    int currentFrame;            // The current frame in the sprite sheet for animation
    double frameTime;            // Time duration for each animation frame
    double timeAccumulator;      // Time accumulator for controlling the frame update based on deltaTime
    int frameWidth, frameHeight; // Width and height of one frame in the sprite sheet
    int totalFrames;             // Total number of frames in the sprite sheet animation
    int score;                   // Player's current score
    int lives;                   // Number of remaining lives for the player
} Player;


int init_player(Player *player, SDL_Renderer *renderer, Map map);
int movePlayer(Player *player, Map *map);
void playerChangeDirection(SDL_Keycode key,Player *player,Map map);
void free_player(Player *player);
void renderPlayer(SDL_Renderer *renderer, Player *player,Map m);
void updatePlayerRenderPosition(Player *player, double deltaTime);
void updatePlayerAnim(Player *player, double deltaTime);
void updatePlayer(Player *player, double deltaTime);
int saveBestScore(Player player);
void movePlayerTo(int x, int y, Player *player, Map *map);
char getNextCell(Player player, Map map);