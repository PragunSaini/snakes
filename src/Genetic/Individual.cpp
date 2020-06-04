#include "Genetic/Individual.hpp"

Individual::Individual() :
    Game() {
    fitness = 0.0;
    score = 0;
}

Individual::Individual(const std::vector<Eigen::MatrixXd> &w, const std::vector<Eigen::VectorXd> &b) :
    Game(w, b) {
    fitness = 0.0;
    score = 0;
}

Individual::Individual(const Individual &other) :
    Game(other),
    fitness(other.fitness),
    score(other.score) {}

Individual &Individual::operator=(const Individual &other) {
    Game::operator=(other);
    fitness = other.fitness;
    score = other.score;
    return *this;
}

bool Individual::operator>(const Individual &other) const {
    return fitness > other.fitness;
}

void Individual::start() {
    initObjects();
    while (snake.alive) {
        snake.look(grid);
        snake.determineMove();
        int foodEaten = snake.move(grid);
        if (foodEaten > 0) {
            foodManager.generateFood(this, foodEaten);
        }
    }
    fitness = snake.fitness;
    score = snake.score;
}