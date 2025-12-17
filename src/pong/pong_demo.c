#include "pong_demo.h"
#include "game_logic.h"
#include "pong_types.h"
#include "render.h"

#include <raylib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

bool IS_RESIZABLE = true;

void pong_demo(WindowOptions *opt) {
    if (IS_RESIZABLE) {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    }
    int screenWidth = opt->width;
    int screenHeight = opt->height;
    int targetFPS = opt->fps;

    InitWindow(screenWidth, screenHeight, "Raylib Window");
    SetTargetFPS(targetFPS);

    const int box_amount = 7;
    const int speed = 200;
    int margin_y = 1.0 / 60 * screenHeight;

    int padding_size = 1.0 / 80 * screenWidth;
    int padding_amount = 1 + box_amount;

    int box_width = (screenWidth - padding_amount * padding_size) / box_amount;
    int box_height = box_width * (9.0 / 16.0);

    int player_widht = 1.0 / 13 * screenWidth;
    int player_height = 1.0 / 30 * screenHeight;
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
            physics(&gameBoxes, &player, &projectile, &state, opt);
            projectile.posX += projectile.speedX * dt;
            projectile.posY += projectile.speedY * dt;
        }

        if (IS_RESIZABLE) {
            opt->height = GetScreenHeight();
            opt->width = GetScreenWidth();
            player.posY = opt->height - margin_y - player_height;
        }

        BeginDrawing();
        switch (state) {
        case RUNNING:
            renderGame(&gameBoxes, &player, &projectile);
            break;
        case LOSS:
            renderLoss(opt);
            break;
        case WIN:
            renderWin(opt);
            break;
        default:
            perror("Encountered invalid state!");
        }

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
}
