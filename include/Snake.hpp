#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "Config.hpp"
#include "NN/NeuralNet.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <utility>
#include <vector>

// forward declaration
class Game;

class Snakebody {
public:
    int x;
    int y;
    bool newblock;
    Direction dir;

    Snakebody(int, int);
    Snakebody(int, int, Direction);
};

class Snake {
    static std::vector<std::pair<int, int>> visionDirs;

    void moveBody();
    void moveHead();
    void increaseLength();
    bool inRange(int, int);
    std::vector<double> objectEncoder(int);
    std::vector<double> getVision();
    void calculateFitness();
    void initFields();
    void die();

public:
    Game *game;
    std::vector<Snakebody> snake;
    NeuralNet net;
    Direction changedir;
    int length;
    int color;
    bool alive;
    int score;
    int steps;
    int stepsSinceFood;
    double fitness;

    Snake(Game *, int = 1);
    void init(const std::vector<NeuralNet::VecWeights> &w = {}, const std::vector<NeuralNet::VecBiases> &b = {});
    // void move(sf::Keyboard::Key); // for keyboard movement
    void update();
};

#endif