#include "Individual.hpp"

Individual::Individual() :
    Game() {
    // init();
    fitness = 0.0;
}

Individual::Individual(const std::vector<NeuralNet::VecWeights> &w, const std::vector<NeuralNet::VecBiases> &b) :
    Game(w, b) {
    // init(w, b);
    fitness = 0.0;
}

Individual::Individual(const Individual &other) :
    Game(other) {
    // init(other.snake.net.weights, other.snake.net.biases);
    fitness = other.fitness;
}

Individual &Individual::operator=(const Individual &other) {
    init(other.snake.net.weights, other.snake.net.biases);
    fitness = other.fitness;
    return *this;
}

bool Individual::operator>(const Individual &other) const {
    return fitness > other.fitness;
}

void Individual::start() {
    while (snake.alive) {
        snake.update();
    }
    fitness = snake.fitness;
}