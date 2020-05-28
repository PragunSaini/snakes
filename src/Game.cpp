#include "Game.hpp"

void Game::init(const std::vector<NeuralNet::VecWeights> &w, const std::vector<NeuralNet::VecBiases> &b) {
    grid = std::vector<std::vector<int>>(Config::ROWS, std::vector<int>(Config::COLS, 0));
    if (!w.empty() && !b.empty()) {
        snake.init(w, b);
    }
    else {
        snake.init();
    }
    foodManager.init();
}

Game::Game() :
    foodManager(this), snake(this) {
    init();
}

Game::Game(const std::vector<NeuralNet::VecWeights> &w, const std::vector<NeuralNet::VecBiases> &b) :
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
