#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <SFML/Graphics.hpp>

namespace Config {
    const int WIDTH = 1280; //1280
    const int HEIGHT = 720; //720

    const int FRAMERATE = 0; // set <= 0 for VSync
    const int SNAKESPEED = 30;

    const int ROWS = 90;  // 90
    const int COLS = 160; // 160

    const int FOODCNT = 25;
} // namespace Config

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT,
    SAME
};

#endif