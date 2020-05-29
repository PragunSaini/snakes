#ifndef RENDER_HPP
#define RENDER_HPP

#include "Game.hpp"
#include "Map.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

class Render : public Game {
private:
    sf::RenderWindow window;
    Map map;
    int tilesize;
    sf::Time snakeSpeed;
    sf::Clock snakeUpdate;
    void draw();
    void handleEvents(sf::Event);

public:
    Render();
    Render(const std::vector<Eigen::MatrixXd> &w, const std::vector<Eigen::VectorXd> &b);
    Render(const Render &);
    void start();
};

#endif