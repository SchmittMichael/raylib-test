#include "pong_demo.h"
#include "raylib.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct obj {
  int posX;
  int posY;
  int widht;
  int height;
};

typedef struct obj Player;
typedef struct obj Box;

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

void physics(GameBoxes *boxes, Player *player, Projectile *projectile,
             int screenWidth, int screenHeight) {

  if (projectile->posX < 0 || projectile->posX > screenWidth) {
    projectile->speedX *= -1;
  }

  if (projectile->posY < 0) projectile->speedY *= -1;

  if (projectile->posY > screenHeight) {
    puts("detected end of game");
    projectile->speedY *= -1;
  }

  for (size_t i = 0; i < boxes->amount; i++) {
    BoxOption *option = &boxes->boxes[i];
    if (!option->exists) continue;

    Box box = option->box;
    if ((projectile->posX > box.posX &&
         projectile->posX < box.posX + box.widht) &&
        (projectile->posY > box.posY &&
         projectile->posY < box.posY + box.height)) {
      puts("Hit box top or bottom");
      option->exists = false;
      projectile->speedY *= -1;
    }
  }

  if (projectile->posY > player->posY &&
      (projectile->posX > player->posX &&
       projectile->posX < player->posX + player->widht)) {
    projectile->speedY *= -1;
  }
}

void pong_demo(int screenWidth, int screenHeight, int targetFPS) {
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

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    if (IsKeyPressed(KEY_Q)) break;
    if (IsKeyDown(KEY_D)) player.posX += speed * dt;
    if (IsKeyDown(KEY_A)) player.posX -= speed * dt;

    physics(&gameBoxes, &player, &projectile, screenWidth, screenHeight);
    projectile.posX += projectile.speedX * dt;
    projectile.posY += projectile.speedY * dt;

    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (size_t i = 0; i < gameBoxes.amount; i++) {
      BoxOption option = boxes[i];
      if (option.exists) {
        Box box = option.box;
        DrawRectangle(box.posX, box.posY, box_width, box_height, PURPLE);
      }
    }
    DrawRectangle(player.posX, player.posY, player.widht, player.height, RED);
    DrawCircle(projectile.posX, projectile.posY, projectile.radius, LIGHTGRAY);

    EndDrawing();
  }

  CloseWindow();
}
