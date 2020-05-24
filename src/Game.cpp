#include "Game.hpp"

Game::Game() : snake(this) {
    // Initialize window
    window.create(sf::VideoMode(Config::WIDTH, Config::HEIGHT), "Snakes");
    if (Config::FRAMERATE > 0) {
        window.setFramerateLimit(Config::FRAMERATE);
    }
    else {
        window.setVerticalSyncEnabled(true);
    }
    tilesize = Config::WIDTH / Config::COLS;

    // Initialize game grid
    for (unsigned int i = 0; i < Config::ROWS; i++) {
        for (unsigned int j = 0; j < Config::COLS; j++) {
            grid[i][j] = 0;
        }
    }

    // Initialize snake
    snake.init();
    snakeSpeed = sf::seconds(1.0 / 30.0);
}

void Game::start() {
    gameloop();
}

void Game::draw() {
    // std::cout << 1.0 / frametime.restart().asSeconds() << std::endl; // framerate
    if (snakeupdate.getElapsedTime() >= snakeSpeed) {
        snake.update();
        snakeupdate.restart();
    }
    window.clear(sf::Color::Black);
    for (Snakebody &snakepart : snake.snake) {
        window.draw(snakepart.body);
    }
    window.display();
}

void Game::eventManager(sf::Event event) {
    switch (event.type) {
    case sf::Event::Closed:
        window.close();
        break;
    case sf::Event::KeyPressed:
        snake.move(event.key.code);
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