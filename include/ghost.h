#pragma once
#include "universal.h"
#include "drawing.h"
#include "player.h"
#include "map.h"
#include <SDL2/SDL.h>

typedef enum
{
    RANDOM
} Movements;

typedef enum
{
    HUNTING,
    EATEABLE,
    EATEN,
} GHOSTSTATE;

typedef struct
{
    int x, y, homeX, homeY;      // Logical position of the ghost on the map (x, y for current position, homeX, homeY for home position)
    double renderX, renderY;     // Rendered (interpolated) position on the screen (smooth movement)
    Direction direction;         // Direction in which the ghost is moving (up, down, left, right)
    double speed;                // Speed of the ghost's movement
    SDL_Texture *texture;        // Texture for the entire ghost (sprite sheet)
    SDL_Texture *textureBlue;    // Alternate texture for the ghost (e.g., for a special state like "eatable")
    SDL_Texture *eatenTexture;   // Texture for the ghost when it's eaten (after being caught by the player)
    int currentFrame;            // The current animation frame
    double frameTime;            // Time duration for each animation frame
    double timeAccumulator;      // Time accumulator for controlling the frame update based on deltaTime
    int frameWidth, frameHeight; // Width and height of one frame in the sprite sheet
    int totalFrames;             // Total number of frames in the sprite sheet animation
    char id;                     // Identifier for the ghost (e.g., 'b' for Blinky, 'n' for Pinky)
    Movements movement;          // Movement pattern of the ghost (e.g., random, chase, scatter)
    GHOSTSTATE state;            // Current state of the ghost (e.g., HUNTING, EATABLE, EATEN)
    char lastCell;               // The last cell (or tile) the ghost was in
    int isMooving;               // Flag indicating whether the ghost is currently moving (1 for moving, 0 for stationary)
    int **wayHome;               // Path (array of cells) to the ghost's home (used for navigation)
    int stepsToHome;             // Number of steps to reach the ghost's home (for navigation)
} Ghost;

int init_ghost(Ghost *ghost, SDL_Renderer *renderer, char *texture, Map map, char indentifier, Movements movement, Player player);
void changeDirection(SDL_Keycode key, Ghost *ghost, Map map);
void renderGhost(SDL_Renderer *renderer, Ghost *ghost, Map m);
int moveGhost(Ghost *ghost, Map *map);
void updateGhostRenderPosition(Ghost *ghost, double deltaTime);
void updateGhostAnim(Ghost *ghost, double deltaTime);
void free_ghost(Ghost *ghost);
void updateGhost(Ghost *ghost, double deltaTime);
void setRandomDirection(Ghost *ghost,Map map);
int moveAllTheGosts(Ghost *ghost, int count, Map *map);
Ghost *getGhostById(Ghost *ghosts,int count, char id);
int findAWayHome(Ghost *ghost,Map map);
int writeStepsToGoHome(Ghost *ghost, Map map);
void freeWayHome(Ghost *ghost);