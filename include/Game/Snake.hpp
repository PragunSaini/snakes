#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "NN/NeuralNet.hpp"
#include <Eigen/Dense>
#include <SFML/Window.hpp>
#include <deque>
#include <utility>
#include <vector>

// forward declaration
class Game;

/*
 * Directions for snake's movements
 */
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT,
    SAME // just a placeholder
};

/**
 * A body part of the snake
 */
class Snakebody {
public:
    int x;
    int y;
    Direction dir;

    Snakebody(int, int, Direction);
};

/*
 * A snake in the game
 */
class Snake {
    static std::vector<std::pair<int, int>> visionDirs; // Directions for snake's vision
    Direction changedir;                                // Used to change direction
    int stepsSinceFood;                                 // Remaining steps since last eaten food

    // Temporary fields to store head movements
    int dx;
    int dy;
    Direction newDir;

    // Utility functions
    Eigen::VectorXd getVision(std::vector<std::vector<int>> &); // get snake's vision vector
    bool inRange(int, int);                                     // checks coordinates in grid
    void die(std::vector<std::vector<int>> &);                  // snake ga shinda

protected:
    int length; // Snake's length
    int color;  // color of the snake

    virtual void calculateFitness(); // calculate fitness of snake's game

public:
    std::deque<Snakebody> snake; // Snake's body
    NeuralNet net;               // NN for the snake
    bool alive;                  // Is snake alive or dead?
    int score;                   // Score of snake = food eaten count
    int steps;                   // Number of steps moved
    double fitness;              // Fitness of snake (calculated after death)

    Snake();
    void initNeuralNet(const std::vector<Eigen::MatrixXd> &w = {}, const std::vector<Eigen::VectorXd> &b = {});
    void initBody(Game *);
    void look(std::vector<std::vector<int>> &);                  // snake looks and get NN's response
    void determineMove();                                        // determine snake's next move
    int move(std::vector<std::vector<int>> &, bool log = false); // move snake
    void handleEvent(sf::Keyboard::Key);                         // for keyboard movement
};

#endif