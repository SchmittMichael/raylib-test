import math
import shutil
from dataclasses import dataclass


@dataclass(frozen=True)
class Color:
    r: int
    b: int
    g: int


def rgb_color(t: float) -> Color:
    return Color(
        int(127.5 + 127.5 * math.sin(t)),
        int(127.5 + 127.5 * math.sin(t + 2.0944)),
        int(127.5 + 127.5 * math.sin(t + 4.1888)),
    )


def draw_rgb(widht: int, height: int):
    try:
        print("\x1b[?25l")
        dt = 0.016
        t = 0
        while True:
            t += 3 * dt
            result = ""
            for y in range(height):
                for _ in range(width):
                    c1 = rgb_color(t)
                    # c2 = rgb_color(t + dt * 10)
                    c2 = c1
                    result += "\x1b[38;2;%d;%d;%dm\x1b[48;2;%d;%d;%dm\u2580\x1b[0m" % (c1.r, c1.g, c1.b, c2.r, c2.g, c2.b)

                if y < height - 2:
                    result += "\n"

            print("\x1b[H", end="")
            print(result, end="", flush=True)
    finally:
        ...
        # print("\x1b[?25h", end="", flush=True)  # show cursor

def draw_color(c: Color, width: int, height: int):
    cond = True
    while cond:
        result = "\x1b[38;2;%d;%d;%dm\x1b[48;2;%d;%d;%dm" % (c.r, c.g, c.b, c.r, c.g, c.b)
        for y in range(height):
            for _ in range(width):
                result += "\u2580"

            if y < height - 1:
                result += "\n"

        result += "\x1bm[0"
        # print("\x1b[H", end="")
        print(result, end="")
        cond =False


width, height = shutil.get_terminal_size()
c = Color(255, 0, 0)
# draw_color(c, width, height)
draw_rgb(width, height)
