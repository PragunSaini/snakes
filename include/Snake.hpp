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
};

class Snake {
    static std::vector<std::pair<int, int>> visionDirs;

    // bool willHitWall();
    void moveBody();
    void moveHead();
    void increaseLength();
    bool inRange(int, int);
    std::vector<double> objectEncoder(int);
    std::vector<double> getVision();

public:
    int length;
    std::vector<Snakebody> snake;
    Game *game;
    Direction changedir;
    int color;
    bool alive;
    NeuralNet net;

    Snake(Game *, int = 1);
    void init(int, int);
    void move(sf::Keyboard::Key);
    void update();
    void die();
};

#endif