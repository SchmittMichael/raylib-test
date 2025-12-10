#include "pong_demo.h"
#include "raylib.h"
#include "types.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
  RUNNING = 0,
  WIN,
  LOSS,
} GameState;

typedef struct {
  int posX;
  int posY;
  int width;
  int height;
} Box;

typedef Box Player;

typedef struct {
  int posX;
  int posY;
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

static inline int clamp(int v, int min, int max) {
  return (v < min) ? min : (v > max ? max : v);
}

static bool circleIntersectsBox(const Projectile *p, const Box *b) {
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
             GameState *state, int screenWidth, int screenHeight) {

  if (projectile->posX < projectile->radius) {
    projectile->speedX *= -1;
    projectile->posX = projectile->radius;
  } else if (projectile->posX + projectile->radius > screenWidth) {
    projectile->speedX *= -1;
    projectile->posX = screenWidth - projectile->radius;
  }

  if (projectile->posY - projectile->radius < 0) {
    projectile->speedY *= -1;
    projectile->posY = projectile->radius;
  } else if (projectile->posY + projectile->radius > screenHeight) {
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

static void renderGame(GameBoxes *gameBoxes, Player *player,
                       Projectile *projectile) {
  const Color player_color = {254, 95, 85, 255};
  const Color ball_color = {189, 213, 234, 255};
  const Color box_color = {73, 88, 103, 255};

  ClearBackground(RAYWHITE);

  for (size_t i = 0; i < gameBoxes->amount; i++) {
    BoxOption option = gameBoxes->boxes[i];
    if (option.exists) {
      Box box = option.box;
      DrawRectangle(box.posX, box.posY, box.width, box.height, box_color);
    }
  }
  DrawRectangle(player->posX, player->posY, player->width, player->height,
                player_color);
  DrawCircle(projectile->posX, projectile->posY, projectile->radius,
             ball_color);
}

static void renderCenteredText(const char *msg, WindowOptions *options) {
  const int fontSize = 30;

  int x = options->width / 2 - MeasureText(msg, fontSize) / 2;
  int y = options->height / 2 - fontSize / 2;

  ClearBackground(RAYWHITE);
  DrawText(msg, x, y, fontSize, RED);
}
static void renderWin(WindowOptions *options) {
  renderCenteredText("YOU WIN!", options);
}

static void renderLoss(WindowOptions *options) {
  renderCenteredText("GAME OVER!", options);
}

void pong_demo(WindowOptions *options) {
  unsigned int screenWidth = options->width;
  unsigned int screenHeight = options->height;
  unsigned int targetFPS = options->fps;

  InitWindow(screenWidth, screenHeight, "Raylib Window");
  SetTargetFPS(targetFPS);

  const int margin_y = 10;
  const int speed = 200;

  const int box_amount = 7;
  const int padding_size = 10;
  int padding_amount = 1 + box_amount;

  int box_width = (screenWidth - padding_amount * padding_size) / box_amount;
  int box_height = box_width * (9.0 / 16.0);

  int player_widht = 60;
  int player_height = 20;
  Player player = {(screenWidth - player_widht) / 2,
                   screenHeight - margin_y - player_height, player_widht,
                   player_height};

  BoxOption boxes[box_amount];
  for (int i = 0; i < box_amount; i++) {
    int pos_x = i * (box_width + padding_size) + padding_size;
    Box box = {pos_x, margin_y, box_width, box_height};
    boxes[i] = (BoxOption){true, box};
  }

  GameBoxes gameBoxes = {box_amount, boxes};

  Projectile projectile = {player.posX + (float)player_widht / 2,
                           player.posY - player.height, 20, speed, speed};

  GameState state = RUNNING;

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    if (IsKeyPressed(KEY_Q)) break;
    if (IsKeyDown(KEY_D)) player.posX += speed * dt;
    if (IsKeyDown(KEY_A)) player.posX -= speed * dt;

    if (state == RUNNING) {
      physics(&gameBoxes, &player, &projectile, &state, screenWidth,
              screenHeight);
      projectile.posX += projectile.speedX * dt;
      projectile.posY += projectile.speedY * dt;
    }

    BeginDrawing();
    switch (state) {
    case RUNNING:
      renderGame(&gameBoxes, &player, &projectile);
      break;
    case LOSS:
      renderLoss(options);
      break;
    case WIN:
      renderWin(options);
      break;
    default:
      fputs("Encountered invalid state!", stderr);
      exit(99);
    }

    EndDrawing();
  }

  CloseWindow();
}
