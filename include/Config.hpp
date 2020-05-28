#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <SFML/Graphics.hpp>
#include <vector>

namespace Config {
    const int WIDTH = 1000;  //1280
    const int HEIGHT = 1000; //720

    const int FRAMERATE = 0; // set <= 0 for VSync
    const int SNAKESPEED = 10;

    const int ROWS = 10; // 90
    const int COLS = 10; // 160

    const int FOODCNT = 1;

    const std::vector<int> LAYERS = {32, 20, 12, 4};

    const int POPSIZE = 500;
    const int NEW_POPSIZE = 200;
    const int GENERATIONS = 1000;
    const double SBX_ETA = 50;
    const double MUTATION_RATE = 0.15;
} // namespace Config

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT,
    SAME
};

#endif