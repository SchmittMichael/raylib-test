#include "game_logic.h"

static inline int clamp(int v, int min, int max) {
    return (v < min) ? min : (v > max ? max : v);
}

bool circleIntersectsBox(const Projectile *p, const Box *b) {
    int closestX = clamp(p->posX, b->posX, b->posX + b->width);
    int closestY = clamp(p->posY, b->posY, b->posY + b->height);

    int dx = p->posX - closestX;
    int dy = p->posY - closestY;
    return (dx * dx + dy * dy) <= (p->radius * p->radius);
}

bool checkWin(GameBoxes *boxes) {
    for (size_t i = 0; i < boxes->amount; i++) {
        if (boxes->boxes[i].exists) {
            return false;
        }
    }
    return true;
}

void physics(GameBoxes *boxes, Player *player, Projectile *projectile,
             GameState *state, WindowOptions *opt) {

    if (projectile->posX - projectile->radius < 0) {
        projectile->speedX *= -1;
        projectile->posX = projectile->radius;
    }
    if (projectile->posX + projectile->radius > opt->width) {
        projectile->speedX *= -1;
        projectile->posX = opt->width - projectile->radius;
    }
    if (projectile->posY - projectile->radius < 0) {
        projectile->speedY *= -1;
        projectile->posY = projectile->radius;
    }
    if (projectile->posY + projectile->radius > opt->height) {
        *state = LOSS;
        return;
    }

    if (circleIntersectsBox(projectile, player)) projectile->speedY *= -1;

    for (size_t i = 0; i < boxes->amount; i++) {
        BoxOption *opt = &boxes->boxes[i];
        if (!opt->exists) continue;

        if (circleIntersectsBox(projectile, &opt->box)) {
            opt->exists = false;

            if (checkWin(boxes)) {
                *state = WIN;
            }

            projectile->speedY = -projectile->speedY;
            break;
        }
    }
}
