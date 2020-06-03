#include "Game/Game.hpp"
#include "Config.hpp"

Game::Game() :
    seed(std::random_device()()),
    gen(seed),
    foodCount(Config::FOODCNT) {
    init();
}

Game::Game(unsigned int seed) :
    seed(seed),
    gen(seed),
    foodCount(Config::FOODCNT) {
    init();
}

Game::Game(const std::vector<Eigen::MatrixXd> &w, const std::vector<Eigen::VectorXd> &b) :
    seed(std::random_device()()),
    gen(seed),
    foodCount(Config::FOODCNT) {
    init(w, b);
}

Game::Game(const Game &other) :
    seed(other.seed),
    gen(seed),
    foodCount(Config::FOODCNT) {
    init(other.snake.net.weights, other.snake.net.biases);
}

void Game::init(const std::vector<Eigen::MatrixXd> &w, const std::vector<Eigen::VectorXd> &b) {
    // Initialize PRNGs
    randX = std::uniform_int_distribution<int>(2, Config::COLS - 3);
    randY = std::uniform_int_distribution<int>(2, Config::ROWS - 3);
    randDir = std::uniform_int_distribution<int>(0, 3);

    // Initialize grid
    grid = std::vector<std::vector<int>>(Config::ROWS, std::vector<int>(Config::COLS, 0));

    // Initialize snake
    snake.initBody(this);
    snake.initNeuralNet(w, b);

    // Initialize food
    foodManager.generateFood(this, foodCount);
}

int Game::getX() {
    return randX(gen);
}

int Game::getY() {
    return randY(gen);
}

int Game::getDir() {
    return randDir(gen);
}

std::mt19937 &Game::getGen() {
    return gen;
}

unsigned int Game::getSeed() {
    return seed;
}