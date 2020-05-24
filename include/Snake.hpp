#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "Config.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

// forward declaration
class Game;

class Snakebody {
public:
    int x;
    int y;
    bool newblock;

    Snakebody(int, int);
};

class Snake {
public:
    int length;
    std::vector<Snakebody> snake;
    Game *game;
    Direction dir;
    int color;

    Snake(Game *, int = 1);
    void init(int, int);
    void move(sf::Keyboard::Key);
    void update();
    void printbody();
};

#endif