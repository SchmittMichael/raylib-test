#include "ball_demo.h"
#include "pong_demo.h"
#include "types.h"

const int screenWidth = 800;
const int screenHeight = 600;
const int targetFPS = 60;

int main(void) {
  WindowOptions opt = {screenWidth, screenHeight, targetFPS};
  // ball_demo();
  pong_demo(&opt);

  return 0;
}
