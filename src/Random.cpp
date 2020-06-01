#include "Random.hpp"

Random::Random() :
    gen(std::mt19937(std::chrono::system_clock::now().time_since_epoch().count())) {
    randX = std::uniform_int_distribution<int>(2, Config::COLS - 3);
    randY = std::uniform_int_distribution<int>(2, Config::ROWS - 3);
    randDir = std::uniform_int_distribution<int>(0, 3);
}

int Random::getX() {
    return randX(gen);
}
int Random::getY() {
    return randY(gen);
}

int Random::getDir() {
	return randDir(gen);
}