#include "Game/Game.hpp"
#include "Config.hpp"
#include <chrono>
#include <fstream>

// SEED GENERATOR
std::mt19937 Seeder::generator = std::mt19937(std::random_device()());
std::uniform_int_distribution<int> Seeder::seeds = std::uniform_int_distribution<int>(0, 1000000);
unsigned int Seeder::getSeed() {
    return seeds(generator);
}

Game::Game() :
    seed(Seeder::getSeed()),
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
    seed(Seeder::getSeed()),
    gen(seed),
    foodCount(Config::FOODCNT) {
    init(w, b);
}

Game::Game(const std::vector<Eigen::MatrixXd> &w, const std::vector<Eigen::VectorXd> &b, unsigned int seed) :
    seed(seed),
    gen(seed),
    foodCount(Config::FOODCNT) {
    init(w, b);
}

Game::Game(const Game &other) :
    seed(other.seed), // copy constructor reseeds the PRNG
    gen(seed),
    foodCount(Config::FOODCNT) {
    init(other.snake.net.weights, other.snake.net.biases); // don't initalize food or snake here since it will change PRNG sequence
}

Game &Game::operator=(const Game &other) {
    seed = other.seed; // copy assignment reseeds the PRNG
    gen = std::mt19937(seed);
    foodCount = Config::FOODCNT;
    init(other.snake.net.weights, other.snake.net.biases); // don't initalize food or snake here since it will change PRNG sequence
    return *this;
}

void Game::init(const std::vector<Eigen::MatrixXd> &w, const std::vector<Eigen::VectorXd> &b) {
    // Initialize PRNGs
    randX = std::uniform_int_distribution<int>(2, Config::COLS - 3);
    randY = std::uniform_int_distribution<int>(2, Config::ROWS - 3);
    randDir = std::uniform_int_distribution<int>(0, 3);

    // Initialize grid
    grid = std::vector<std::vector<int>>(Config::ROWS, std::vector<int>(Config::COLS, 0));

    // Initialize snake
    snake.initNeuralNet(w, b);
}

void Game::initObjects() {
    // Initialize snake and food
    snake.initBody(this);
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

void Game::reseed() {
    seed = Seeder::getSeed();
    gen = std::mt19937(seed);
}

void Game::saveToFile(int offset) {
    std::ofstream ofs("saves/Seed" + std::to_string(offset) + ".seed");
    ofs << seed << "\n";
    ofs.close();
    snake.net.saveToFile(offset);
}

void Game::loadFromFile(int offset) {
    std::ifstream ifs("saves/Seed" + std::to_string(offset) + ".seed");
    ifs >> seed;
    gen = std::mt19937(seed);
    ifs.close();
    snake.net.loadFromFile(offset);
}