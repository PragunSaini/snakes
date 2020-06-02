#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <SFML/Graphics.hpp>
#include <vector>

namespace Config {
    const int WIDTH = 1000;  //1280
    const int HEIGHT = 1000; //720

    const int FRAMERATE = 0; // set <= 0 for VSync
    const double SNAKESPEED = 30;

    const int ROWS = 100; // 90
    const int COLS = 100; // 160

    const int FOODCNT = 1;

    // const std::vector<int> LAYERS = {32, 18, 8, 4};
    const std::vector<int> LAYERS = {32, 20, 12, 4};
    // const std::vector<int> LAYERS = {32, 18, 4};


    const int POPSIZE = 500;    // was 500
    const int NEW_POPSIZE = 1000;   // was 1000
    const int GENERATIONS = 30000;  // was 30000
    const double SBX_ETA = 100; // was 200
    const double MUTATION_RATE = 0.05;  // was 0.1


    // const int POPSIZE = 700;    // was 500
    // const int NEW_POPSIZE = 1400;   // was 1000
    // const int GENERATIONS = 50000;  // was 30000
    // const double SBX_ETA = 250; // was 200
    // const double MUTATION_RATE = 0.07;  // was 0.1

} // namespace Config

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT,
    SAME
};

#endif