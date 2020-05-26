#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <SFML/Graphics.hpp>

namespace Config {
    const int WIDTH = 100;  //1280
    const int HEIGHT = 100; //720

    const int FRAMERATE = 0; // set <= 0 for VSync
    const int SNAKESPEED = 30;

    const int ROWS = 10; // 90
    const int COLS = 10; // 160

    const int FOODCNT = 1;

    const int POPSIZE = 200;
} // namespace Config

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT,
    SAME
};

#endif