#include "Game.hpp"

Game::Game() :
    grid(std::vector<std::vector<int>>(Config::ROWS, std::vector<int>(Config::COLS, 0))),
    tilesize(Config::WIDTH / Config::COLS),
    map(this) {

    // Initialize window
    window.create(sf::VideoMode(Config::WIDTH, Config::HEIGHT), "Snakes");
    if (Config::FRAMERATE > 0) {
        window.setFramerateLimit(Config::FRAMERATE);
    }
    else {
        window.setVerticalSyncEnabled(true);
    }

    // Initialize snake
    for (int i = 0; i < 1; i++) {
        snake.push_back(Snake(this));
        snake[i].init(2 * i);
    }

    grid[0][0] = 1;
    map.changeColor(0, 0);

    snakeSpeed = sf::seconds(1.0 / 30.0);
}

void Game::start() {
    gameloop();
}

void Game::draw() {
    if (snakeupdate.getElapsedTime() >= snakeSpeed) {
        window.setTitle("Snakes | FPS : " + std::to_string((int)(1.0 / frametime.restart().asSeconds())));
        for (Snake &snek : snake) {
            snek.update();
        }
        snakeupdate.restart();
    }
    else {
        frametime.restart();
    }

    window.clear(sf::Color::Black);
    window.draw(map);
    window.display();
}

void Game::eventManager(sf::Event event) {
    switch (event.type) {
    case sf::Event::Closed:
        window.close();
        break;
    case sf::Event::KeyPressed:
        for (Snake &snek : snake) {
            snek.move(event.key.code);
        }
        break;
    default:
        break;
    }
}

void Game::gameloop() {
    frametime.restart();
    snakeupdate.restart();

    // start the game loop
    while (window.isOpen()) {

        // Poll events
        sf::Event event;
        while (window.pollEvent(event)) {
            eventManager(event);
        }

        // Draw display
        draw();
    }
}