#ifndef GAME_HPP
#define GAME_HPP

#include "Config.hpp"
#include "Map.hpp"
#include "Random.hpp"
#include "Snake.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>

class Game {
public:
    sf::RenderWindow window;
    std::vector<std::vector<int>> grid;
    int tilesize;
    Map map;
    sf::Clock frametime;
    sf::Time snakeSpeed;
    sf::Clock snakeupdate;
    std::vector<Snake> snake;
    sf::Font font;
    Random rand;

    Game();
    void gameloop();
    void start();
    void draw();
    void eventManager(sf::Event);
    void initSnake();
};

#endif