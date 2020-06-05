#include "Tournament/Tournament.hpp"
#include "Config.hpp"

Tournament::Tournament(const std::vector<Eigen::MatrixXd> &w, const std::vector<Eigen::VectorXd> &b) :
    Game(w, b),
    playerCount(Config::PLAYERCNT),
    players(playerCount),
    started(false) {
    foodCount = Config::FOODCNT_T;
    for (Player &player : players) {
        player.initNeuralNet(w, b);
    }
    initializeWindow();
}

void Tournament::initObjects() {
    for (Player &player : players) {
        player.initBody(this);
    }
    foodManager.generateFood(this, foodCount);
}

void Tournament::initializeWindow() {
    // Set window size and framerate
    window.create(sf::VideoMode(Config::WIDTH, Config::HEIGHT), "Snakes");
    if (Config::FRAMERATE > 0) {
        window.setFramerateLimit(Config::FRAMERATE);
    }
    else {
        window.setVerticalSyncEnabled(true);
    }
    // Set snake update speed
    snakeSpeed = sf::seconds(1.0 / Config::SNAKESPEED);

    // Load fonts
    if (!font.loadFromFile("./assets/Roboto-Regular.ttf")) {
        window.setTitle("Snakes (failed to load fonts)");
        return;
    }

    // Initialize score text
    bestScore.setFont(font);
    bestScore.setCharacterSize(20);
    bestScore.setFillColor(sf::Color::White);
    bestScore.setString("Press Enter to start");
    bestScore.setPosition(sf::Vector2f(5, 5));
}

void Tournament::draw() {
    if (started && snakeUpdate.getElapsedTime() >= snakeSpeed) {
        for (Player &player : players) {
            if (player.alive) {
                player.look(grid);
                player.determineMove();
            }
        }
        int foodEaten = 0;
        for (Player &player : players) {
            if (player.alive)
                foodEaten += player.move(grid);
        }
        foodManager.generateFood(this, foodEaten);
        int bestscore = 0;
        for (Player &player : players) {
            bestscore = bestscore > player.score ? bestscore : player.score;
        }
        bestScore.setString("Best Score : " + std::to_string(bestscore));
        snakeUpdate.restart();
    }

    map.updateMap(grid);

    window.setTitle("Snakes | FPS : " + std::to_string((int)(1.0 / fps.restart().asSeconds())));

    window.clear();
    window.draw(map);
    window.draw(bestScore);
    window.display();
}

void Tournament::start() {
    initObjects();

    snakeUpdate.restart();
    fps.restart();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    started = true;
                }
            }
        }
        draw();
    }
}

void Tournament::loadFromFile() {
    int i = 0;
    for (Player &player : players) {
        player.net.loadFromFile(0); // just loading from single file for all snakes
        i++;
    }
}