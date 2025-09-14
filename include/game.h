#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>

// Debug mode
#define DEBUG 1

// Screen dimension constants
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define BLOCK_SIZE 64

// Colors
#define COLOR_WHITE 0x00FFFFFF // White: (255,255,255)
#define COLOR_BLACK 0x00000000 // Black: (0,0,0)
#define COLOR_RED   0x00FF0000 // Red: (255,0,0) 
#define COLOR_GREEN 0x0000FF00 // Green: (0,255,0)
#define COLOR_BLUE  0x000000FF // Blue: (0,0,255)

// Game state
extern bool isGameRunning;

// Player
typedef struct s_player {
    // Player position
    float x;
    float y;
    float angle; // 
} t_player;

extern void init_player(t_player *player);
extern void move_player(t_player *player, bool up, bool down, bool left, bool right, bool rotate_left, bool rotate_right);

#endif // GAME_H