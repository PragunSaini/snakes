#ifndef GAME_HPP
#define GAME_HPP

#include "Game/Food.hpp"
#include "Game/Snake.hpp"
#include <Eigen/Dense>
#include <vector>

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
    // Grid, NN, Snake and Food Initializer
    void init(const std::vector<Eigen::MatrixXd> &w = {}, const std::vector<Eigen::VectorXd> &b = {});

public:
    FoodManager foodManager;            // for food generation
    Snake snake;                        // Snake
    std::vector<std::vector<int>> grid; // 2D game grid
    int foodCount;                      // count of food on grid

    Game();
    Game(unsigned int seed);
    Game(const std::vector<Eigen::MatrixXd> &w, const std::vector<Eigen::VectorXd> &b);
    Game(const Game &);
    virtual void start() = 0; // abstract function to start the game

    // Functions for RNG
    int getX();
    int getY();
    int getDir();
    std::mt19937 &getGen();
    unsigned int getSeed();
};

#endif