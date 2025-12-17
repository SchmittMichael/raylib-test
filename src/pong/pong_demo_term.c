#include "game_logic.h"
#include "pong_demo.h"
#include "pong_types.h"
#include "render.h"

#include <fcntl.h>
#include <raylib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

static struct termios old_termios;

static inline Color pixelVal(int x, int y, int width, const Color *pixels) {
    return pixels[y * width + x];
}

void draw(const Color *pixels, const WindowOptions *opt) {
    const int width = opt->width;
    const int height = opt->height;

    const size_t bufsize = (size_t)width * height * 40 + height + 1;
    char *buffer = malloc(bufsize);
    if (!buffer) {
        return;
    }

    size_t offset = 0;
    buffer[0] = '\0';

    for (int y = 0; y < height; y += 2) {
        for (int x = 0; x < width; x++) {
            Color top = pixelVal(x, y, width, pixels);
            Color bottom =
                (y + 1 < height) ? pixelVal(x, y + 1, width, pixels) : top;

            offset +=
                snprintf(buffer + offset, bufsize - offset,
                         "\x1b[38;2;%d;%d;%dm"
                         "\x1b[48;2;%d;%d;%dm"
                         "\u2580"
                         "\x1b[0m",
                         top.r, top.g, top.b, bottom.r, bottom.g, bottom.b);
        }

        if (y < height - 2) {
            buffer[offset++] = '\n';
        }
    }

    printf("\x1b[H");
    fputs(buffer, stdout);
    fflush(stdout);

    free(buffer);
}

void set_window_opts(WindowOptions *opt) {
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
        perror("ioctl");
        exit(1);
    }

    opt->width = ws.ws_col;
    opt->height = ws.ws_row * 2;
}

void enable_raw_input(void) {
    struct termios new_termios;

    tcgetattr(STDIN_FILENO, &old_termios);
    new_termios = old_termios;

    new_termios.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

int read_key(void) {
    fd_set set;
    struct timeval tv = {0, 0};

    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);

    if (select(STDIN_FILENO + 1, &set, NULL, NULL, &tv) > 0) {
        char c;
        if (read(STDIN_FILENO, &c, 1) == 1) return c;
    }
    return -1;
}

void disable_raw_input(void) { tcsetattr(STDIN_FILENO, TCSANOW, &old_termios); }

void pong_demo_term(void) {
    WindowOptions options = {0};
    WindowOptions *opt = &options;
    set_window_opts(opt);

    int screenWidth = opt->width;
    int screenHeight = opt->height;

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
                             player.posY - player.height, 5, speed, speed};

    GameState state = RUNNING;
    // workaround
    SetTraceLogLevel(LOG_NONE);
    InitWindow(1, 1, "hidden");
    SetWindowState(FLAG_WINDOW_HIDDEN);

    RenderTexture2D target = LoadRenderTexture(opt->width, opt->height);
    enable_raw_input();
    while (true) {
        float dt = dt = 0.016;
        int key = read_key();
        if (key == 'q') break;
        if (key == 'a') player.posX -= speed * dt;
        if (key == 'd') player.posX += speed * dt;

        if (state == RUNNING) {
            physics(&gameBoxes, &player, &projectile, &state, opt);
            projectile.posX += projectile.speedX * dt;
            projectile.posY += projectile.speedY * dt;
        }

        Image image = LoadImageFromTexture(target.texture);
        BeginTextureMode(target);
        ClearBackground(BLACK);
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

        EndTextureMode();

        Color *pixels = LoadImageColors(image);
        draw(pixels, opt);

        UnloadImageColors(pixels);
        // UnloadImage(image);
        // UnloadRenderTexture(target);
    }
    UnloadRenderTexture(target);
    disable_raw_input();
}
