#ifndef RENDER_H
#define RENDER_H

#include "pong_types.h"
#include "types.h"
#include <raylib.h>

static const Color PLAYER_COLOR = {254, 95, 85, 255};
static const Color BALL_COLOR = {189, 213, 234, 255};
static const Color BOX_COLOR = {73, 88, 103, 255};

void renderGame(GameBoxes *gameBoxes, Player *player, Projectile *projectile);
void renderWin(WindowOptions *opt);
void renderLoss(WindowOptions *opt);

#endif
