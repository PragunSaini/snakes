#include "Render.hpp"

Render::Render() :
    Game() {
    window.create(sf::VideoMode(Config::WIDTH, Config::HEIGHT), "Snakes");
    if (Config::FRAMERATE > 0) {
        window.setFramerateLimit(Config::FRAMERATE);
    }
    else {
        window.setVerticalSyncEnabled(true);
    }

    snakeSpeed = sf::seconds(1.0 / Config::SNAKESPEED);
}

Render::Render(const std::vector<Eigen::MatrixXd> &w, const std::vector<Eigen::VectorXd> &b) :
    Game(w, b) {
    window.create(sf::VideoMode(Config::WIDTH, Config::HEIGHT), "Snakes");
    if (Config::FRAMERATE > 0) {
        window.setFramerateLimit(Config::FRAMERATE);
    }
    else {
        window.setVerticalSyncEnabled(true);
    }

    snakeSpeed = sf::seconds(1.0 / Config::SNAKESPEED);
}

Render::Render(const Render &other) :
    Game(other) {
    window.create(sf::VideoMode(Config::WIDTH, Config::HEIGHT), "Snakes");
    if (Config::FRAMERATE > 0) {
        window.setFramerateLimit(Config::FRAMERATE);
    }
    else {
        window.setVerticalSyncEnabled(true);
    }

    snakeSpeed = sf::seconds(1.0 / Config::SNAKESPEED);
}

void Render::draw() {
    if (snakeUpdate.getElapsedTime() >= snakeSpeed) {
        snake.update();
        snakeUpdate.restart();
    }
    window.clear();
    map.updateMap(grid);
    window.draw(map);
    window.display();
}

void Render::handleEvents(sf::Event event) {
    switch (event.type) {
    case sf::Event::Closed:
        window.close();
        break;

    default:
        break;
    }
}

void Render::start() {
    snakeUpdate.restart();
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            handleEvents(event);
        }
        draw();
    }
}