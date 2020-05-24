#ifndef RANDOM_HPP
#define RANDOM_HPP

#include "Config.hpp"
#include <chrono>
#include <random>

class Random {
public:
    std::uniform_int_distribution<int> randX;
    std::uniform_int_distribution<int> randY;
    std::mt19937 gen;

    Random();
    int getX();
    int getY();
};

#endif