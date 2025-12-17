#ifndef PONG_TYPES_H
#define PONG_TYPES_H

#include <stdbool.h>
#include <stddef.h>

typedef enum {
    RUNNING = 0,
    WIN,
    LOSS,
} GameState;

typedef struct {
    float posX;
    float posY;
    int width;
    int height;
} Box;

typedef Box Player;

typedef struct {
    float posX;
    float posY;
    int radius;
    int speedX;
    int speedY;
} Projectile;

typedef struct {
    bool exists;
    Box box;
} BoxOption;

typedef struct {
    size_t amount;
    BoxOption *boxes;
} GameBoxes;

#endif
