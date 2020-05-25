#include "Snake.hpp"
#include "Game.hpp"

// Initialize a snake body part at given coords
Snakebody::Snakebody(int x, int y) :
    x(x), y(y), newblock(true) {}

Snake::Snake(Game *game, int color) :
    game(game), color(color), alive(true) {
}

void Snake::init(int x, int y) {
    // Initialize a snake with one body part
    length = 1;
    snake.push_back(Snakebody(x, y));
    snake[0].newblock = false;
    game->grid[y][x] = color;
    dir = Direction::DOWN;
}

void Snake::update() {
    if (alive) {
        if (!snake[length - 1].newblock)
            game->grid[snake[length - 1].y][snake[length - 1].x] = 0;

        for (int i = length - 1; i >= 1; i--) {
            snake[i] = snake[i - 1];
            game->grid[snake[i].y][snake[i].x] = color;
        }

        switch (changedir) {
        case Direction::UP:
            if (dir != Direction::DOWN) {
                dir = Direction::UP;
            }
            break;

        case Direction::DOWN:
            if (dir != Direction::UP) {
                dir = Direction::DOWN;
            }
            break;

        case Direction::LEFT:
            if (dir != Direction::RIGHT) {
                dir = Direction::LEFT;
            }
            break;

        case Direction::RIGHT:
            if (dir != Direction::LEFT) {
                dir = Direction::RIGHT;
            }
            break;

        default:
            break;
        }

        changedir = Direction::SAME;

        switch (dir) {
        case Direction::UP:
            snake[0].y = snake[0].y == 0 ? 0 : snake[0].y - 1;
            break;

        case Direction::DOWN:
            snake[0].y = snake[0].y == Config::ROWS - 1 ? Config::ROWS - 1 : snake[0].y + 1;
            break;

        case Direction::LEFT:
            snake[0].x = snake[0].x == 0 ? 0 : snake[0].x - 1;
            break;

        case Direction::RIGHT:
            snake[0].x = snake[0].x == Config::COLS - 1 ? Config::COLS - 1 : snake[0].x + 1;
            break;

        default:
            break;
        }

        if (game->grid[snake[0].y][snake[0].x] > 0 && game->grid[snake[0].y][snake[0].x] < 10) {
            alive = false;
            die();
        }
        else if (game->grid[snake[0].y][snake[0].x] == -1) {
            snake.push_back(Snakebody(0, 0));
            length++;
            game->grid[snake[0].y][snake[0].x] = color;
            game->foodManager.regenerate();
        }
        else {
            game->grid[snake[0].y][snake[0].x] = color;
        }
    }
}

void Snake::die() {
    color = 10 + color;
    for (int i = 1; i < length; i++) {
        game->grid[snake[i].y][snake[i].x] = color;
    }
}

void Snake::move(sf::Keyboard::Key key) {
    // Move the snake
    switch (key) {
    case sf::Keyboard::Up:
        if (dir != Direction::DOWN) {
            changedir = Direction::UP;
        }
        break;

    case sf::Keyboard::Down:
        if (dir != Direction::UP) {
            changedir = Direction::DOWN;
        }
        break;

    case sf::Keyboard::Left:
        if (dir != Direction::RIGHT) {
            changedir = Direction::LEFT;
        }
        break;

    case sf::Keyboard::Right:
        if (dir != Direction::LEFT) {
            changedir = Direction::RIGHT;
        }
        break;

    default:
        break;
    }
}
