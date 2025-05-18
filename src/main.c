#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "../include/game.h"

// Constants for time management
#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

// Global variables
bool isGameRunning = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int lastFrameTime = 0;

// Function prototypes
bool initialize_window(void);
void process_input(void);
void update(float deltaTime);
void render(void);
void cleanup(void);

void put_pixel(SDL_Renderer* renderer, int x, int y, Uint32 color) {
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
        return; // Out of bounds
    }
    // Set the color for drawing operations (R, G, B, A)
    SDL_SetRenderDrawColor(renderer, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, 255);
    // Draw a single point at the specified (x, y) position
    SDL_RenderDrawPoint(renderer, x, y);
}


void draw_square(SDL_Renderer* renderer, int x, int y, int size, Uint32 color) {
    for (int i = 0; i < size; i++) {
        put_pixel(renderer, x + i, y, color);
    }    
    for (int i = 0; i < size; i++) {
        put_pixel(renderer, x, y + i, color);
    }
    for (int i = 0; i < size; i++) {
        put_pixel(renderer, x + i, y + size, color);
    }
    for (int i = 0; i < size; i++) {
        put_pixel(renderer, x + size, y + i, color);
    }
}

int main(int argc, char **argv) {
    isGameRunning = initialize_window();
    
    // Game loop
    while (isGameRunning) {
        // Calculate delta time
        int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - lastFrameTime);
        if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
            SDL_Delay(timeToWait);
        }
        
        // Delta time in seconds
        float deltaTime = (SDL_GetTicks() - lastFrameTime) / 1000.0f;
        lastFrameTime = SDL_GetTicks();
        
        // Game loop phases
        process_input();
        update(deltaTime);
        render();
    }
    
    cleanup();
    return 0;
}

bool initialize_window(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return false;
    }
    
    window = SDL_CreateWindow(
        "RaycastingDemo",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    
    if (!window) {
        fprintf(stderr, "Error creating SDL Window: %s\n", SDL_GetError());
        return false;
    }
    
    // Create a renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL Renderer: %s\n", SDL_GetError());
        return false;
    }
    
    return true;
}

void process_input(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isGameRunning = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    isGameRunning = false;
                break;
        }
    }
}

void update(float deltaTime) {
    // Game logic update code will go here
}

void render(void) {
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    // Game rendering code will go here
    draw_square(renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 10, COLOR_GREEN);
    
    // Present the rendered frame
    SDL_RenderPresent(renderer);
}

void cleanup(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}