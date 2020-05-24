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

    Snakebody(int, int, sf::Color);
};

class Snake {
public:
    unsigned int length;
    std::vector<Snakebody> snake;
    Game *game;

    Snake(Game *);
    void init(int a);
    void move(sf::Keyboard::Key);
    void update();
    void printbody();
    Direction dir;
};

#endif