#include "Game.hpp"

void Game::init(const std::vector<Eigen::MatrixXd> &w, const std::vector<Eigen::VectorXd> &b) {
    grid = std::vector<std::vector<int>>(Config::ROWS, std::vector<int>(Config::COLS, 0));
    snake.init(w, b);
    foodManager.init();
}

Game::Game() :
    foodManager(this), snake(this) {
    init();
}

Game::Game(const std::vector<Eigen::MatrixXd> &w, const std::vector<Eigen::VectorXd> &b) :
    foodManager(this),
    snake(this) {
    init(w, b);
}

Game::Game(const Game &other) :
    foodManager(this),
    snake(this) {
    init(other.snake.net.weights, other.snake.net.biases);
}

Game &Game::operator=(const Game &other) {
    init(other.snake.net.weights, other.snake.net.biases);
    return *this;
}
