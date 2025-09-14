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

// Fix the fishbowl effect by adjusting the distance based on the angle
float fixed_distance(float x1, float y1, float x2, float y2) {
    float detla_x = x2 - x1;
    float detla_y = y2 - y1;
    float angle = atan2(detla_y, detla_x) - g_player.angle;
    float fixed_distance = sqrt(detla_x * detla_x + detla_y * detla_y) * cos(angle);
    return fixed_distance;
}

void draw_line(SDL_Renderer* render, float start_x, int step, Uint32 color) {
    float cos_angle = cos(start_x);
    float sin_angle = sin(start_x);
    float ray_x = g_player.x;
    float ray_y = g_player.y;

    while (!touch(ray_x, ray_y)) {
        //put_pixel(renderer, (int)ray_x, (int)ray_y, color);
        // Move the ray forward in the direction of the player's angle
        ray_x += cos_angle;
        ray_y += sin_angle;
    }

    float distance = fixed_distance(g_player.x, g_player.y, ray_x, ray_y);
    float height = (BLOCK_SIZE / distance) * (SCREEN_WIDTH / 2) ;
    int start_y = (SCREEN_HEIGHT - height) / 2;
    int end = start_y + height;
    while (start_y < end)
    {
        put_pixel(renderer, step, start_y, COLOR_BLUE);
        ++start_y;
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
    // draw_square(renderer, g_player.x, g_player.y, 10, COLOR_GREEN);
    // draw_map(renderer, g_map);

    float fraction = M_PI / 3 / SCREEN_WIDTH; // 60 degrees field of view
    float start_angle = g_player.angle - (M_PI / 6); // Start angle
    int i = 0;
    while (i < SCREEN_WIDTH) {
        draw_line(renderer, start_angle, i, COLOR_RED);
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