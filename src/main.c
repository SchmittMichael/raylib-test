#include "raylib.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

const int screenWidth = 800;
const int screenHeight = 600;
const int targetFPS = 60;

void movePos(float dx, float dy, Vector2 *pos) {
  float newX = pos->x + dx;
  float newY = pos->y + dy;

  if (newX < 0) newX = 0;
  if (newX > screenWidth) newX = screenWidth;

  if (newY < 0) newY = 0;
  if (newY > screenHeight) newY = screenHeight;

  pos->x = newX;
  pos->y = newY;
}

Color getRGBColor(float t) {
  return (Color){127.5f + 127.5f * sinf(t), 127.5f + 127.5f * sinf(t + 2.0944f),
                 127.5f + 127.5f * sinf(t + 4.1888f), 255};
}

int main(void) {
  InitWindow(screenWidth, screenHeight, "Raylib Window");
  SetTargetFPS(targetFPS);

  Vector2 playerPos = {(float)screenWidth / 2, (float)screenHeight / 2};

  int speed = 100;
  Vector2 funnyBallPos = {0, 0};
  Vector2 funnyBallSpeed = {speed * 5, speed * 5};
  float t = 0;

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    if (IsKeyDown(KEY_W)) movePos(0, -speed * dt, &playerPos);
    if (IsKeyDown(KEY_A)) movePos(-speed * dt, 0, &playerPos);
    if (IsKeyDown(KEY_S)) movePos(0, speed * dt, &playerPos);
    if (IsKeyDown(KEY_D)) movePos(speed * dt, 0, &playerPos);

    if (IsKeyPressed(KEY_Q)) break;

    if (funnyBallPos.x > screenWidth || funnyBallPos.x < 0)
      funnyBallSpeed.x *= -1;
    if (funnyBallPos.y > screenHeight || funnyBallPos.y < 0)
      funnyBallSpeed.y *= -1;

    funnyBallPos.x += funnyBallSpeed.x * dt;
    funnyBallPos.y += funnyBallSpeed.y * dt;

    t += 3 * dt;

    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("Super tolle Demo !!!", 250, 300, 30, DARKGRAY);

    DrawCircleV(playerPos, 10, BLUE);
    DrawCircleV(funnyBallPos, 20, getRGBColor(t));

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
