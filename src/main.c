#include <stdio.h>
#include "raylib.h"

int main() {
  printf("Hello to my beautifull programm\n");


  InitWindow(800, 600, "Raylib + Meson");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello from Raylib + Meson!", 200, 300, 30, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
