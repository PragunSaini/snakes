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
    unsigned int x;
    unsigned int y;
    sf::RectangleShape body;
    Game *game;

    Snakebody(Game *);
    Snakebody(Game *, unsigned int, unsigned int, sf::Color);
};

class Snake {
public:
    unsigned int length;
    std::vector<Snakebody> snake;
    Game *game;

    Snake(Game *);
    void init();
    void move(sf::Keyboard::Key);
    void update();
    void printbody();
    Direction dir;
};

#endif