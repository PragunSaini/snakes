#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <SFML/Graphics.hpp>
#include <vector>

namespace Config {
    // Screen Params
    const int WIDTH = 600;  //1280
    const int HEIGHT = 600; //720

    const int FRAMERATE = 0; // set <= 0 for VSync
    const double SNAKESPEED = 100;

    const int ROWS = 10; // 90
    const int COLS = 10; // 160

    // NN layers
    const std::vector<int> LAYERS = {32, 20, 12, 4};

    // GA Params
    const int POPSIZE = 500;
    const int NEW_POPSIZE = 1000;
    const int GENERATIONS = 10000;
    const double SBX_ETA = 100;
    const double MUTATION_RATE = 0.05;

    // Snake Vision Params
    const bool CLOCKWISE_VISION = false; // vision is respective to direction of snake
    const int VISION_TYPE = 1;           // 1 for distance, 0 for binary

    // Food Count (for single snake training)
    const int FOODCNT = 1;

    // Food Count (for tournament snake training)
    const int FOODCNT_T = 200;

} // namespace Config

#endif