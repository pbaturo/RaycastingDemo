#include "../include/game.h"

// Constants for time management
#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

// Global variables
bool isGameRunning = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int lastFrameTime = 0;
//Game variables
t_player g_player;
char **g_map;


// Function prototypes
bool initialize_window(void);
void initilize_game(void);
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

void draw_map(SDL_Renderer* renderer, char **map) {
    char **_map = map;
    int color = COLOR_BLUE;
    for (int y = 0; _map[y]; y++) {
        for (int x=0; _map[y][x]; x++) {
            if (map[y][x] == '1') {
                draw_square(renderer,x *64, y*64, 64, color);
            }
        }
    }
}

bool touch(float px, float py) {
    int x = (int)(px / BLOCK_SIZE);
    int y = (int)(py / BLOCK_SIZE);
    if (g_map[y][x] == '1') {
        return true; // Collision detected
    }
    return false; // No collision
}

void draw_line(SDL_Renderer* render, float angle, Uint32 color) {
    float cos_angle = cos(angle);
    float sin_angle = sin(angle);
    float ray_x = g_player.x;
    float ray_y = g_player.y;

    while (!touch(ray_x, ray_y)) {
        put_pixel(renderer, (int)ray_x, (int)ray_y, color);
        // Move the ray forward in the direction of the player's angle
        ray_x += cos_angle;
        ray_y += sin_angle;
    }
}

char **get_map(void) {
    char **map = malloc(sizeof(char *) * 11);
    for (int i = 0; i < 10; i++) {
        map[i] = malloc(sizeof(char) * 11);
    }
    map[0] = "1111111111111";
    map[1] = "1000000000001";
    map[2] = "1001000000001";
    map[3] = "1000000100001";
    map[4] = "1000001000001";
    map[5] = "1000000000001";
    map[6] = "1000000000001";
    map[7] = "1001000000001";
    map[8] = "1000000000001";
    map[9] = "1111111111111";
    map[10] = NULL;
    return map;
}

int main(int argc, char **argv) {
    // Initialize Game
    isGameRunning = initialize_window();
    initilize_game();
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

void initilize_game(void) {
    init_player(&g_player);
    g_map = get_map();
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
                if (event.key.keysym.sym == SDLK_UP)
                    move_player(&g_player, true, false, false, false, false, false);
                if (event.key.keysym.sym == SDLK_DOWN)
                    move_player(&g_player, false, true, false, false, false, false);
                if (event.key.keysym.sym == SDLK_LEFT)
                    move_player(&g_player, false, false, true, false, false, false);
                if (event.key.keysym.sym == SDLK_RIGHT)
                    move_player(&g_player, false, false, false, true, false, false);
                if (event.key.keysym.sym == SDLK_a)
                    move_player(&g_player, false, false, false, false, true, false);
                if (event.key.keysym.sym == SDLK_d)
                    move_player(&g_player, false, false, false, false, false, true);
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
    draw_square(renderer, g_player.x, g_player.y, 10, COLOR_GREEN);
    draw_map(renderer, g_map);

    // // Present the rendered frame
    // float ray_x = g_player.x;
    // float ray_y = g_player.y;
    // float cos_angle = cos(g_player.angle);
    // float sin_angle = sin(g_player.angle);

    // while (!touch(ray_x, ray_y)) {
    //     put_pixel(renderer, (int)ray_x, (int)ray_y, COLOR_RED);
    //     // Move the ray forward in the direction of the player's angle
    //     ray_x += cos_angle;
    //     ray_y += sin_angle;
    // }

    float fraction = M_PI / 3 / SCREEN_WIDTH; // 60 degrees field of view
    float start_angle = g_player.angle - (M_PI / 6); // Start angle
    int i = 0;
    while (i < SCREEN_WIDTH) {
        draw_line(renderer, start_angle, COLOR_RED);
        start_angle += fraction;
        ++i;
    }
    SDL_RenderPresent(renderer);
}

void cleanup(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}