#include "Tournament/Player.hpp"
#include "Game/Game.hpp"

bool Player::isValidPosition(int x, int y, Direction dir, Game *game) {
    switch (dir) {
    case Direction::LEFT:
        if (game->grid[y][x] != 0 ||
            game->grid[y][x + 1] != 0 ||
            game->grid[y][x + 2] != 0) {
            return false;
        }
        return true;
    case Direction::RIGHT:
        if (game->grid[y][x] != 0 ||
            game->grid[y][x - 1] != 0 ||
            game->grid[y][x - 2] != 0) {
            return false;
        }
        return true;
    default:
        return true;
    }
}

void Player::initBody(Game *game) {
    int x, y;
    Direction dir;
    do {
        x = game->getX();
        y = game->getY();
        dir = (Direction)((game->getDir() % 2) * 2 + 1);
    } while (!isValidPosition(x, y, dir, game));

    length = 3;
    snake.push_back(Snakebody(x, y, dir));
    game->grid[y][x] = color;

    if (dir == Direction::RIGHT) {
        snake.push_back(Snakebody(x - 1, y, dir));
        game->grid[snake.back().y][snake.back().x] = color;
        snake.push_back(Snakebody(x - 2, y, dir));
        game->grid[snake.back().y][snake.back().x] = color;
    }
    else if (dir == Direction::LEFT) {
        snake.push_back(Snakebody(x + 1, y, dir));
        game->grid[snake.back().y][snake.back().x] = color;
        snake.push_back(Snakebody(x + 2, y, dir));
        game->grid[snake.back().y][snake.back().x] = color;
    }
}

// (WIP)
void Player::calculateFitness() {
    fitness = steps +
              (std::pow(2.0, (double)score) + std::pow((double)score, 2.1) * 500) -
              (std::pow(0.1 * steps, 1.3) * std::pow((double)score, 1.1));
    fitness = std::max(fitness, .1);
    // fitness = 0.25 * steps + std::pow((double)score, 3);
    // fitness = score * 2.0;
    // fitness = std::pow(2.0, (double)score) + std::pow((double)score, 2.1) - std::pow(steps * 0.25, 1.5);
}