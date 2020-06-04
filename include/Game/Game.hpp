#ifndef GAME_HPP
#define GAME_HPP

#include "Game/Food.hpp"
#include "Game/Snake.hpp"
#include <Eigen/Dense>
#include <vector>

/*
 * Used to generate seeds
 */
namespace Seeder {
    extern std::mt19937 generator;
    extern std::uniform_int_distribution<int> seeds;
    extern unsigned int getSeed();
}; // namespace Seeder

/*
 * Abstract class to handle games
 */
class Game {
private:
    // Members for RNG
    unsigned int seed; // seed for reproducing results
    std::mt19937 gen;
    std::uniform_int_distribution<int> randX;
    std::uniform_int_distribution<int> randY;
    std::uniform_int_distribution<int> randDir;

protected:
    // Grid and NN
    void init(const std::vector<Eigen::MatrixXd> &w = {}, const std::vector<Eigen::VectorXd> &b = {});

public:
    FoodManager foodManager;            // for food generation
    Snake snake;                        // Snake
    std::vector<std::vector<int>> grid; // 2D game grid
    int foodCount;                      // count of food on grid

    Game();
    Game(unsigned int);
    Game(const std::vector<Eigen::MatrixXd> &w, const std::vector<Eigen::VectorXd> &b);
    Game(const std::vector<Eigen::MatrixXd> &w, const std::vector<Eigen::VectorXd> &b, unsigned int);
    Game(const Game &);            // copy constructor - copies seed too
    Game &operator=(const Game &); // copy constructor - copies seed too

    // Functions for RNG
    int getX();
    int getY();
    int getDir();
    std::mt19937 &getGen();
    unsigned int getSeed();
    void reseed();      // to reseed the game
    void initObjects(); // initializes snake and food
    void saveToFile(int);
    void loadFromFile(int);

    virtual void start() = 0; // abstract function to start the game
};

#endif