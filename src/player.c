#include "../include/game.h"

void init_player(t_player *player) {
    player->x = SCREEN_WIDTH / 2;
    player->y = SCREEN_HEIGHT / 2;
}

void move_player(t_player *player, bool up, bool down, bool left, bool right) {
    int speed = 10;
    
    if (up) {
        player->y -= speed;
    }
    if (down) {
        player->y += speed;
    }
    if (left) {
        player->x -= speed;
    }
    if (right) {
        player->x += speed;
    }
}