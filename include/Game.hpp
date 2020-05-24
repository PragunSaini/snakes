#ifndef GAME_HPP
#define GAME_HPP

#include "Config.hpp"
#include "Map.hpp"
#include "Snake.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <random>
#include <vector>

class Game {
public:
    sf::RenderWindow window;
    // unsigned int grid[Config::ROWS][Config::COLS];
    std::vector<std::vector<int>> grid;
    unsigned int tilesize;
    Map map;
    sf::Clock frametime;
    sf::Time snakeSpeed;
    sf::Clock snakeupdate;
    std::vector<Snake> snake;
    sf::Font font;

    Game();
    void gameloop();
    void start();
    void draw();
    void eventManager(sf::Event);
    void initSnake();
};

#endif