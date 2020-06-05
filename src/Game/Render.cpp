#include "Game/Render.hpp"
#include "Config.hpp"

Render::Render(bool vision) :
    Game(),
    vision(vision),
    started(false) {
    initializeWindow();
}

Render::Render(unsigned int seed, bool vision) :
    Game(seed),
    vision(vision),
    started(false) {
    initializeWindow();
}

Render::Render(const std::vector<Eigen::MatrixXd> &w, const std::vector<Eigen::VectorXd> &b) :
    Game(w, b),
    vision(true),
    started(false) {
    initializeWindow();
}

Render::Render(const std::vector<Eigen::MatrixXd> &w, const std::vector<Eigen::VectorXd> &b, unsigned int seed) :
    Game(w, b, seed),
    vision(true),
    started(false) {
    initializeWindow();
}

Render::Render(const Render &other) :
    Game(other),
    vision(other.vision),
    started(false) {
    initializeWindow();
}

Render &Render::operator=(const Render &other) {
    Game::operator=(other);
    vision = other.vision;
    started = false;
    initializeWindow();
    return *this;
}

void Render::initializeWindow() {
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
    score.setFont(font);
    score.setCharacterSize(20);
    score.setFillColor(sf::Color::White);
    score.setString("Press Enter to start");
    score.setPosition(sf::Vector2f(5, 5));
}

void Render::draw() {
    if (started && snakeUpdate.getElapsedTime() >= snakeSpeed) {
        // Move the snake
        if (vision) {
            snake.look(grid);
        }
        snake.determineMove();
        int foodEaten = snake.move(grid);
        foodManager.generateFood(this, foodEaten);
        score.setString("Score : " + std::to_string(snake.score));
        snakeUpdate.restart();
    }

    // Update the map
    map.updateMap(grid);

    // Update FPS
    window.setTitle("Snakes | FPS : " + std::to_string((int)(1.0 / fps.restart().asSeconds())));

    // Draw the new game state
    window.clear();
    window.draw(map);
    window.draw(score);
    window.display();
}

void Render::handleEvents(sf::Event event) {
    switch (event.type) {

    case sf::Event::Closed:
        window.close();
        break;

    case sf::Event::KeyPressed:
        snake.handleEvent(event.key.code);
        if (event.key.code == sf::Keyboard::Enter) {
            started = true;
        }
        break;

    default:
        break;
    }
}

void Render::start() {
    // Init food and snake
    initObjects();

    // Start the clocks
    snakeUpdate.restart();
    fps.restart();

    // Start game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            handleEvents(event);
        }
        draw();
    }
}