#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <SFML/Graphics.hpp>

namespace Config {
    const unsigned int WIDTH = 1280;
    const unsigned int HEIGHT = 720;
    const unsigned int FRAMERATE = 0; // set <= 0 for VSync

    const unsigned int ROWS = 90;
    const unsigned int COLS = 160;
} // namespace Config

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SAME
};

#endif