#include "render.h"

void renderGame(GameBoxes *gameBoxes, Player *player, Projectile *projectile) {
    ClearBackground(RAYWHITE);

    for (size_t i = 0; i < gameBoxes->amount; i++) {
        BoxOption option = gameBoxes->boxes[i];
        if (option.exists) {
            Box box = option.box;
            DrawRectangle(box.posX, box.posY, box.width, box.height, BOX_COLOR);
        }
    }
    DrawRectangle(player->posX, player->posY, player->width, player->height,
                  PLAYER_COLOR);
    DrawCircle(projectile->posX, projectile->posY, projectile->radius,
               BALL_COLOR);
}

void renderCenteredText(const char *msg, WindowOptions *options) {
    const int fontSize = 30;

    int x = options->width / 2 - MeasureText(msg, fontSize) / 2;
    int y = options->height / 2 - fontSize / 2;

    ClearBackground(RAYWHITE);
    DrawText(msg, x, y, fontSize, RED);
}

void renderWin(WindowOptions *options) {
    renderCenteredText("YOU WIN!", options);
}

void renderLoss(WindowOptions *options) {
    renderCenteredText("GAME OVER!", options);
}
