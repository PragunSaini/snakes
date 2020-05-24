#include "Snake.hpp"
#include "Game.hpp"

Snakebody::Snakebody(Game *game) : Snakebody(game, 0, 0, sf::Color::Yellow) {}

// Initialize a snake body part at given coords
Snakebody::Snakebody(Game *game, unsigned int _x, unsigned int _y, sf::Color col = sf::Color::Yellow)
    : game(game) {
    x = _x;
    y = _y;
    body = sf::RectangleShape(sf::Vector2f(x, y));
    body.setSize(sf::Vector2f(game->tilesize, game->tilesize));
    body.setPosition(x * game->tilesize, y * game->tilesize);
    body.setFillColor(col);
}

Snake::Snake(Game *game) : game(game) {}

void Snake::init(int a) {
    // Initialize a snake with one body part
    length = 1;

    snake.push_back(Snakebody(game, a, 0));
    // snake.push_back(Snakebody(game, 0, 1));
    // snake.push_back(Snakebody(game, 0, 0));
    dir = Direction::DOWN;
}

void Snake::update() {
    for (int i = length - 1; i >= 1; i--) {
        snake[i] = snake[i - 1];
        snake[i].body.setPosition(snake[i].x * game->tilesize, snake[i].y * game->tilesize);
    }

    switch (dir) {
    case Direction::UP:
        snake[0].y--;
        break;

    case Direction::DOWN:
        snake[0].y++;
        break;

    case Direction::LEFT:
        snake[0].x--;
        break;

    case Direction::RIGHT:
        snake[0].x++;
        break;

    default:
        break;
    }

    snake[0].body.setPosition(snake[0].x * game->tilesize, snake[0].y * game->tilesize);
    // printbody(); // debugging
}

void Snake::printbody() {
    std::cout << "-----------------" << std::endl;
    for (unsigned int i = 0; i < length; i++) {
        std::cout << snake[i].x << " | " << snake[i].y << "\n";
    }
    std::cout << "-----------------" << std::endl;
}

void Snake::move(sf::Keyboard::Key key) {
    // Move the snake
    switch (key) {
    case sf::Keyboard::Up:
        if (dir != Direction::DOWN) {
            dir = Direction::UP;
        }
        break;

    case sf::Keyboard::Down:
        if (dir != Direction::UP) {
            dir = Direction::DOWN;
        }
        break;

    case sf::Keyboard::Left:
        if (dir != Direction::RIGHT) {
            dir = Direction::LEFT;
        }
        break;

    case sf::Keyboard::Right:
        if (dir != Direction::LEFT) {
            dir = Direction::RIGHT;
        }
        break;

    case sf::Keyboard::Enter:
        snake.push_back(Snakebody(game, -1, -1));
        length++;
        break;

    default:
        break;
    }
}
