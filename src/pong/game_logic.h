#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "../types.h"
#include "pong_types.h"
#include <stdbool.h>

void physics(GameBoxes *boxes, Player *player, Projectile *projectile,
             GameState *state, WindowOptions *opt);

#endif
