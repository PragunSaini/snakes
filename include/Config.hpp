#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <SFML/Graphics.hpp>
#include <vector>

namespace Config {
    const int WIDTH = 900;  //1280
    const int HEIGHT = 900; //720

    const int FRAMERATE = 0; // set <= 0 for VSync
    const double SNAKESPEED = 10;

    const int ROWS = 10; // 90
    const int COLS = 10; // 160

    const int FOODCNT = 1;

    // const std::vector<int> LAYERS = {32, 18, 8, 4};
    const std::vector<int> LAYERS = {32, 20, 12, 4};
    // const std::vector<int> LAYERS = {32, 18, 4};

    const int POPSIZE = 500;           // was 500
    const int NEW_POPSIZE = 1000;      // was 1000
    const int GENERATIONS = 10000;     // was 30000
    const double SBX_ETA = 100;        // was 200
    const double MUTATION_RATE = 0.05; // was 0.1

    const bool CLOCKWISE_VISION = true;
    const int VISION_TYPE = 1; // 1 for distance, 0 for binary

} // namespace Config

#endif