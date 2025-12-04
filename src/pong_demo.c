#include "pong_demo.h"
#include "raylib.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

void pong_demo(int screenWidth, int screenHeight, int targetFPS) {
  InitWindow(screenWidth, screenHeight, "Raylib Window");
  SetTargetFPS(targetFPS);

  int box_amount = 4;
  int padding_amount = 2 + box_amount / 2 + 1;
  int padding_size = 10;

  int box_width = (screenWidth - padding_amount * padding_size) / box_amount;
  int box_height = 50;

  printf("box_width=%d, padding_amount=%d\n", box_width, padding_amount);

  bool printed = false;

  while (!WindowShouldClose()) {

    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (int i = 0; i < box_amount; i++) {
      int box_x = i * box_width + padding_size;
      if (!printed) { printf("Box_%d: %d\n", i, box_x); }
      printed = true;
      DrawRectangle(box_x, 10, box_width, box_height, PURPLE);
    }

    EndDrawing();
  }

  CloseWindow();
}
