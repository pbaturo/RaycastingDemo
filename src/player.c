#include "../include/game.h"

void init_player(t_player *player) {
    player->x = SCREEN_WIDTH / 2;
    player->y = SCREEN_HEIGHT / 2;
    player->angle = M_PI / 2;
}

void move_player(t_player *player, bool up, bool down, bool left, bool right, bool rotate_left, bool rotate_right) {
    int speed = 5;
    float angle_speed = 0.1;
    float cos_angle = cos(player->angle);
    float sin_angle = sin(player->angle);
    if (rotate_left) {
        player->angle -= angle_speed;
    } else {
        player->angle += angle_speed;
    }
    if (player->angle < 0) {
        player->angle = 2 * M_PI;
    }
    if (player->angle > 2 * M_PI) {
        player->angle = 0;
    }
    
    if (up) {
        player->x += cos_angle * speed;
        player->y += sin_angle * speed;
    }
    if (down) {
        player->x -= cos_angle * speed;
        player->y -= sin_angle * speed;
    }
    if (left) {
        player->x += cos_angle * speed;
        player->y -= sin_angle * speed;
    }
    if (right) {
        player->x -= cos_angle * speed;
        player->y += sin_angle * speed;
    }
}