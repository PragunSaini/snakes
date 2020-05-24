#ifndef GAME_HPP
#define GAME_HPP

#include "Config.hpp"
#include "Snake.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

class Game {
public:
    sf::RenderWindow window;
    unsigned int grid[Config::ROWS][Config::COLS];
    unsigned int tilesize;
    sf::Clock frametime;
    sf::Time snakeSpeed;
    sf::Clock snakeupdate;
    Snake snake;

    Game();
    void gameloop();
    void start();
    void draw();
    void eventManager(sf::Event);
    void initSnake();
};

#endif