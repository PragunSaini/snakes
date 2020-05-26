#include "Game.hpp"

Game::Game() :
    grid(std::vector<std::vector<int>>(Config::ROWS, std::vector<int>(Config::COLS, 0))),
    tilesize(Config::WIDTH / Config::COLS),
    // map(this),
    foodManager(this),
    snake(this) {

    // Initialize window
    // window.create(sf::VideoMode(Config::WIDTH, Config::HEIGHT), "Snakes");
    // if (Config::FRAMERATE > 0) {
    //     window.setFramerateLimit(Config::FRAMERATE);
    // }
    // else {
    //     window.setVerticalSyncEnabled(true);
    // }

    // Initialize snake
    std::srand(time(NULL));
    // for (int i = 0; i < 1; i++) {
    //     snake.push_back(Snake(this, std::rand() % 3 + 1));
    //     snake[i].init(rand.getX(), rand.getY());
    // }

    // map.updateMap();

    snakeSpeed = sf::seconds(1.0 / Config::SNAKESPEED);

    foodManager.init();

    snake.init(rand.getX(), rand.getY());
}

void Game::start() {
    gameloop();
}

// void Game::draw() {
//     window.setTitle("Snakes | FPS : " + std::to_string((int)(1.0 / frametime.restart().asSeconds())));
//     if (snakeupdate.getElapsedTime() >= snakeSpeed) {
//         for (Snake &snek : snake) {
//             snek.update();
//         }
//         snakeupdate.restart();
//     }
//     map.updateMap();
//     window.clear(sf::Color::Black);
//     window.draw(map);
//     window.display();
// }

void Game::move() {
    // if (snakeupdate.getElapsedTime() >= snakeSpeed) {
    snake.update();
    // snakeupdate.restart();
    // }
}

// void Game::eventManager(sf::Event event) {
//     switch (event.type) {
//     case sf::Event::Closed:
//         window.close();
//         break;
//     case sf::Event::KeyPressed:
//         for (Snake &snek : snake) {
//             snek.move(event.key.code);
//         }
//         break;
//     default:
//         break;
//     }
// }

void Game::gameloop() {
    frametime.restart();
    snakeupdate.restart();

    while (snake.alive) {
        move();
    }

    // start the game loop
    // while (window.isOpen()) {
    //     // Poll events
    //     sf::Event event;
    //     while (window.pollEvent(event)) {
    //         eventManager(event);
    //     }

    //     // Draw display
    //     draw();
    // }
}