#include "Game/Render.hpp"
#include "Config.hpp"

Render::Render(bool vision) :
    Game(),
    vision(vision) {
    initializeWindow();
}

Render::Render(const std::vector<Eigen::MatrixXd> &w, const std::vector<Eigen::VectorXd> &b) :
    Game(w, b),
    vision(true) {
    initializeWindow();
}

Render::Render(const Render &other) :
    Game(other),
    vision(other.vision) {
    initializeWindow();
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
    score.setCharacterSize(24);
    score.setFillColor(sf::Color::White);
    score.setString("Score : ");
    score.setPosition(sf::Vector2f(5, 5));
}

void Render::draw() {
    if (snakeUpdate.getElapsedTime() >= snakeSpeed) {
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

    default:
        break;
    }
}

void Render::start() {
    // Start the clocks
    snakeUpdate.restart();
    fps.restart();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            handleEvents(event);
        }
        draw();
    }
}