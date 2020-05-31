#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <SFML/Graphics.hpp>
#include <vector>

namespace Config {
    const int WIDTH = 500;  //1280
    const int HEIGHT = 500; //720

    const int FRAMERATE = 0; // set <= 0 for VSync
    const int SNAKESPEED = 10;

    const int ROWS = 10; // 90
    const int COLS = 10; // 160

    const int FOODCNT = 1;

    // const std::vector<int> LAYERS = {32, 18, 8, 4};
    const std::vector<int> LAYERS = {32, 20, 12, 4};

    // First with these params in savesb
    // const int POPSIZE = 1000;
    // const int NEW_POPSIZE = 3000;
    // const int GENERATIONS = 10000;
    // const double SBX_ETA = 200;
    // const double MUTATION_RATE = 0.10;

    // then with these
    // const int POPSIZE = 1000;
    // const int NEW_POPSIZE = 3000;
    // const int GENERATIONS = 20000;
    // const double SBX_ETA = 300;
    // const double MUTATION_RATE = 0.125;

    const int POPSIZE = 1000;
    const int NEW_POPSIZE = 3000;
    const int GENERATIONS = 2000;
    const double SBX_ETA = 230;
    const double MUTATION_RATE = 0.125;

} // namespace Config

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT,
    SAME
};

#endif

// #ifndef CONFIG_HPP
// #define CONFIG_HPP

// #include <SFML/Graphics.hpp>
// #include <vector>

// namespace Config {
//     const int WIDTH = 1000;  //1280
//     const int HEIGHT = 1000; //720

//     const int FRAMERATE = 0; // set <= 0 for VSync
//     const int SNAKESPEED = 10;

//     const int ROWS = 10; // 90
//     const int COLS = 10; // 160

//     const int FOODCNT = 1;

//     const std::vector<int> LAYERS = {32, 18, 8, 4};

//     const int POPSIZE = 1000;
//     const int NEW_POPSIZE = 2000;
//     const int GENERATIONS = 3000;
//     const double SBX_ETA = 200;
//     const double MUTATION_RATE = 0.10;
// } // namespace Config

// enum Direction {
//     UP,
//     RIGHT,
//     DOWN,
//     LEFT,
//     SAME
// };

// #endif