#include "Random.hpp"

Random::Random() :
    gen(std::mt19937(std::chrono::system_clock::now().time_since_epoch().count())) {
    randX = std::uniform_int_distribution<int>(0, Config::COLS - 1);
    randY = std::uniform_int_distribution<int>(0, Config::ROWS - 1);
}

int Random::getX() {
    return randX(gen);
}
int Random::getY() {
    return randY(gen);
}