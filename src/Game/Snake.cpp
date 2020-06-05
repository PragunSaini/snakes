#include "Game/Snake.hpp"
#include "Config.hpp"
#include "Game/Game.hpp"
#include <iostream>

Snakebody::Snakebody(int x, int y, Direction dir) :
    x(x), y(y), dir(dir) {}

// Directions for snake vision
std::vector<std::pair<int, int>> Snake::visionDirs = {
    {0, -1}, // UP
    {1, -1}, // UP-RIGHT
    {1, 0},  // RIGHT
    {1, 1},  // DOWN-RIGHT
    {0, 1},  // DOWN
    {-1, 1}, // DOWN-LEFT,
    {-1, 0}, // LEFT
    {-1, -1} // UP-LEFT
};

Snake::Snake() :
    changedir(Direction::SAME),
    stepsSinceFood(Config::ROWS * Config::COLS),
    color(Seeder::getSeed() % 4 + 1),
    alive(true),
    score(0),
    steps(0),
    fitness(0.0) {}

void Snake::initBody(Game *game) {
    int x = game->getX();
    int y = game->getY();

    length = 3;
    snake.push_back(Snakebody(x, y, (Direction)(game->getDir())));
    game->grid[y][x] = color;

    if (snake.front().dir == Direction::RIGHT) {
        snake.push_back(Snakebody(x - 1, y, Direction::RIGHT));
        game->grid[snake.back().y][snake.back().x] = color;
        snake.push_back(Snakebody(x - 2, y, Direction::RIGHT));
        game->grid[snake.back().y][snake.back().x] = color;
    }
    else if (snake.front().dir == Direction::LEFT) {
        snake.push_back(Snakebody(x + 1, y, Direction::LEFT));
        game->grid[snake.back().y][snake.back().x] = color;
        snake.push_back(Snakebody(x + 2, y, Direction::LEFT));
        game->grid[snake.back().y][snake.back().x] = color;
    }
    else if (snake.front().dir == Direction::UP) {
        snake.push_back(Snakebody(x, y + 1, Direction::UP));
        game->grid[snake.back().y][snake.back().x] = color;
        snake.push_back(Snakebody(x, y + 2, Direction::UP));
        game->grid[snake.back().y][snake.back().x] = color;
    }
    else {
        snake.push_back(Snakebody(x, y - 1, Direction::DOWN));
        game->grid[snake.back().y][snake.back().x] = color;
        snake.push_back(Snakebody(x, y - 2, Direction::DOWN));
        game->grid[snake.back().y][snake.back().x] = color;
    }
}

void Snake::initNeuralNet(const std::vector<Eigen::MatrixXd> &w, const std::vector<Eigen::VectorXd> &b) {
    // Default initalize NN weights
    if (w.size() == 0 && b.size() == 0) {
        net = NeuralNet(Config::LAYERS);
    }
    // or from params
    else {
        net = NeuralNet(Config::LAYERS, w, b);
    }
}

// Get a one hot encoded boolean array of snakes vision
// Looks in 8 directions and reports the distances/presence of food or body parts
// Also encodes directions of head and tail
Eigen::VectorXd Snake::getVision(std::vector<std::vector<int>> &grid) {
    std::vector<double> res;
    res.reserve(32);

    int visInd = 0;
    if (Config::CLOCKWISE_VISION) {
        visInd = ((int)snake[0].dir) * 2;
    }

    for (int i = 0; i < 8; i++) {
        std::pair<int, int> dir = visionDirs[visInd % 8];
        visInd++;
        std::vector<double> encoding(3, 0.0); // wall, apple, body
        int dx = snake.front().x;
        int dy = snake.front().y;
        int distance = 0;
        bool bodyFound = false;
        bool foodFound = false;
        do {
            distance++;
            dx += dir.first;
            dy += dir.second;
            // If wall reached
            if (!inRange(dx, dy)) {
                encoding[0] = 1.0 / distance;
                break;
            }
            // If first food found
            if (!foodFound && grid[dy][dx] == -1) {
                foodFound = true;
                encoding[1] = Config::VISION_TYPE ? 1.0 / distance : 1.0;
            }
            // If snake body part found
            if (!bodyFound && grid[dy][dx] > 0 && grid[dy][dx] < 10) {
                bodyFound = true;
                encoding[2] = Config::VISION_TYPE ? 1.0 / distance : 1.0;
            }
        } while (true);
        res.insert(res.end(), encoding.begin(), encoding.end());
    }

    // Encode head directions
    std::vector<double> headDir(4, 0);
    headDir[(int)snake.front().dir] = 1;
    res.insert(res.end(), headDir.begin(), headDir.end());

    // Encode tail directions
    std::vector<double> tailDir(4, 0);
    tailDir[(int)snake.back().dir] = 1;
    res.insert(res.end(), tailDir.begin(), tailDir.end());

    return Eigen::VectorXd::Map(res.data(), res.size());
}

void Snake::look(std::vector<std::vector<int>> &grid) {
    // Hide tail
    grid[snake.back().y][snake.back().x] = 0;

    // Find direction to move in using NN
    Eigen::VectorXd res = net.feedforward(getVision(grid));
    int dist;
    res.maxCoeff(&dist);
    changedir = (Direction)dist;

    // Show tail
    grid[snake.back().y][snake.back().x] = color;
}

void Snake::determineMove() {
    Direction dir = snake.front().dir;

    // Update direction
    switch (changedir) {
    case Direction::UP:
        if (dir != Direction::DOWN)
            dir = Direction::UP;
        break;

    case Direction::DOWN:
        if (dir != Direction::UP)
            dir = Direction::DOWN;
        break;

    case Direction::LEFT:
        if (dir != Direction::RIGHT)
            dir = Direction::LEFT;
        break;

    case Direction::RIGHT:
        if (dir != Direction::LEFT)
            dir = Direction::RIGHT;
        break;

    default:
        break;
    }
    changedir = Direction::SAME;

    // Get current head locations
    dx = snake.front().x;
    dy = snake.front().y;
    newDir = dir;

    // Update new head coordinates
    switch (dir) {
    case Direction::UP:
        dy--;
        break;

    case Direction::DOWN:
        dy++;
        break;

    case Direction::LEFT:
        dx--;
        break;

    case Direction::RIGHT:
        dx++;
        break;

    default:
        break;
    }
}

// Check coords in grid
bool Snake::inRange(int x, int y) {
    if (x >= 0 && y >= 0 && y < Config::ROWS && x < Config::COLS)
        return true;
    return false;
}

int Snake::move(std::vector<std::vector<int>> &grid, bool log) {
    bool foodEaten = false;
    if (alive) {
        // If snake out of grid or eats snake body, DIE
        if (!inRange(dx, dy) || (grid[dy][dx] > 0 && grid[dy][dx] < 10)) {
            die(grid);
            if (log) {
                if (!inRange(dx, dy)) {
                    std::cout << "DEATH BY OUT OF GRID : " << dx << "," << dy << "\n";
                }
                else {
                    std::cout << "DEATH BY SELF EATING : " << dx << "," << dy << "\n";
                }
            }
        }
        // Check if eating food
        else if (grid[dy][dx] == -1) {
            foodEaten = true;
            length++;
            // Add new head
            snake.push_front(Snakebody(dx, dy, newDir));
            grid[dy][dx] = color;
            // Update params
            score++;
            stepsSinceFood = Config::ROWS * Config::COLS;
            // Regenerate food
        }
        // Else just move snake
        else {
            // Insert new head
            snake.push_front(Snakebody(dx, dy, newDir));
            grid[snake.front().y][snake.front().x] = color;
            // Remove tail
            grid[snake.back().y][snake.back().x] = 0;
            snake.pop_back();
        }
        // If snake takes too long to eat, it dies
        if (stepsSinceFood <= 0) {
            die(grid);
            if (log) {
                std::cout << "DEATH BY STARVATION"
                          << "\n";
            }
        }
        // Update params
        stepsSinceFood--;
        steps++;
    }
    return foodEaten ? 1 : 0;
}

void Snake::die(std::vector<std::vector<int>> &grid) {
    alive = false;
    color = 10 + color;
    for (int i = 0; i < length; i++) {
        // Head may not be in grid
        if (i == 0 && inRange(snake[i].x, snake[i].y))
            grid[snake[i].y][snake[i].x] = color;
        if (i != 0)
            grid[snake[i].y][snake[i].x] = color;
    }
    calculateFitness();
}

void Snake::calculateFitness() {
    fitness = steps +
              (std::pow(2.0, (double)score) + std::pow((double)score, 2.1) * 500) -
              (std::pow(0.25 * steps, 1.3) * std::pow((double)score, 1.2));
    fitness = std::max(fitness, .1);
}

// For keyboard movement
void Snake::handleEvent(sf::Keyboard::Key key) {
    switch (key) {
    case sf::Keyboard::Up:
        changedir = Direction::UP;
        break;

    case sf::Keyboard::Down:
        changedir = Direction::DOWN;
        break;

    case sf::Keyboard::Left:
        changedir = Direction::LEFT;
        break;

    case sf::Keyboard::Right:
        changedir = Direction::RIGHT;
        break;

    default:
        break;
    }
}