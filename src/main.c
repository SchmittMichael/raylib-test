// #include "ball_demo.h"
#include "pong_demo.h"

const int screenWidth = 800;
const int screenHeight = 600;
const int targetFPS = 60;

int main(void) {
  // ball_demo();
  pong_demo(screenWidth, screenHeight, targetFPS);

  return 0;
}
