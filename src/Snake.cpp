#include "Snake.hpp"
#include "Game.hpp"

Snakebody::Snakebody(int x, int y) :
    x(x), y(y), newblock(true) {}

Snakebody::Snakebody(int x, int y, Direction dir) :
    x(x), y(y), newblock(true), dir(dir) {}

// Directions for snake vision
std::vector<std::pair<int, int>> Snake::visionDirs = {
    {0, -1}, // UP
    {1, -1}, // UP-RIGHT
    {1, 0},  // RIGHT
    {1, 1},  // DOWN-RIGHT
    {0, 1},  // DOWN
    {-1, 1}, // DOWN-LEFT,
    {-1, 0}, // LEFT
    {-1, 1}  // UP-LEFT
};

Snake::Snake(Game *game, int color) :
    game(game), color(color), alive(true) {
}

void Snake::initFields() {
    int x = game->rand.getX();
    int y = game->rand.getY();

    length = 1;
    snake.push_back(Snakebody(x, y, (Direction)(game->rand.getX() % 4)));
    snake[0].newblock = false;
    game->grid[y][x] = color;

    score = 0;
    steps = 0;
    stepsSinceFood = 0;
}

void Snake::init(const std::vector<NeuralNet::VecWeights> &w, const std::vector<NeuralNet::VecBiases> &b) {
    initFields();
    // Default initalize NN weights or from params
    if (!w.empty() && !b.empty()) {
        net = NeuralNet(Config::LAYERS, w, b);
    }
    else {
        net = NeuralNet(Config::LAYERS);
    }
}

// Move snakes body except the head
void Snake::moveBody() {
    if (!snake[length - 1].newblock)
        game->grid[snake[length - 1].y][snake[length - 1].x] = 0;

    for (int i = length - 1; i >= 1; i--) {
        snake[i] = snake[i - 1];
        game->grid[snake[i].y][snake[i].x] = color;
    }
}

// Move snakes head
void Snake::moveHead() {
    Direction &dir = snake[0].dir;
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
}

// Add snakebody to snake
void Snake::increaseLength() {
    snake.push_back(Snakebody(0, 0, snake[length - 1].dir));
    length++;
}

// Check snake in grid
bool Snake::inRange(int x, int y) {
    if (x >= 0 && y >= 0 && y < Config::ROWS && x < Config::COLS)
        return true;
    return false;
}

// Update snakes position (aka move snake)
void Snake::update() {
    if (alive) {
        stepsSinceFood++;

        // Find direction to move in using NN
        std::vector<double> res = net.feedforward(getVision());
        int dist = std::distance(res.begin(), std::max_element(res.begin(), res.end()));
        changedir = (Direction)dist;

        // Move snake
        moveBody();
        moveHead();
        steps++;

        // Check if out of grid or eating a snake
        if (!inRange(snake[0].x, snake[0].y) || (game->grid[snake[0].y][snake[0].x] > 0 && game->grid[snake[0].y][snake[0].x] < 10)) {
            die();
        }
        // Check if eating food
        else if (game->grid[snake[0].y][snake[0].x] == -1) {
            stepsSinceFood = 0;
            increaseLength();
            score++;
            game->foodManager.regenerate();
            game->grid[snake[0].y][snake[0].x] = color;
        }
        else {
            game->grid[snake[0].y][snake[0].x] = color;
        }

        // If snake takes too long to eat, it dies
        if (stepsSinceFood >= Config::COLS * Config::ROWS) {
            die();
        }
    }
}

void Snake::die() {
    alive = false;
    color = 10 + color;
    for (int i = 0; i < length; i++) {
        if (i == 0 && inRange(snake[i].x, snake[i].y))
            game->grid[snake[i].y][snake[i].x] = color;
        if (i != 0)
            game->grid[snake[i].y][snake[i].x] = color;
    }
    calculateFitness();
}

void Snake::calculateFitness() {
    fitness = steps +
              (std::pow(2, score) + std::pow(score, 2.1) * 500) -
              (std::pow(0.25 * steps, 1.3) * std::pow(score, 1.2));
    fitness = std::max(fitness, .1);
    // fitness = steps + 100 * score - steps * 0.5;
    // fitness = score;
}

// Utility function to encode an object in vision
std::vector<double> Snake::objectEncoder(int x) {
    std::vector<double> res(3, 0);
    if (x == -1) {
        res[0] = 1;
    }
    else if (x > 0 && x < 10) {
        res[1] = 1;
    }
    else {
        res[2] = 1;
    }
    return res;
}

// Get a one hot encoded boolean array of snakes vision
// Looks in 8 directions clockwise and reports first thing it sees
// Also encodes directions of head and tail
std::vector<double> Snake::getVision() {
    std::vector<double> res;
    res.reserve(32);

    int visInd = ((int)snake[0].dir) * 2;
    for (int i = 0; i < 8; i++) {
        std::pair<int, int> d = visionDirs[visInd % 8];
        visInd++;
        int dx = snake[0].x;
        int dy = snake[0].y;
        int obj = 0;
        do {
            dx += d.first;
            dy += d.second;
            if (!inRange(dx, dy)) {
                break;
            }
            if (game->grid[dy][dx] != 0 && game->grid[dy][dx] < 10) {
                obj = game->grid[dy][dx];
                break;
            }
        } while (true);
        std::vector<double> encoding = objectEncoder(obj);
        res.insert(res.end(), encoding.begin(), encoding.end());
    }
    std::vector<double> headDir(4, 0);
    headDir[(int)snake[0].dir] = 1;
    res.insert(res.end(), headDir.begin(), headDir.end());

    std::vector<double> tailDir(4, 0);
    tailDir[(int)snake[length - 1].dir] = 1;
    res.insert(res.end(), tailDir.begin(), tailDir.end());
    return res;
}

// For keyboard movement
// void Snake::move(sf::Keyboard::Key key) {
//     Direction dir = snake[0].dir;
//     // Move the snake
//     switch (key) {
//     case sf::Keyboard::Up:
//         if (dir != Direction::DOWN) {
//             changedir = Direction::UP;
//         }
//         break;

//     case sf::Keyboard::Down:
//         if (dir != Direction::UP) {
//             changedir = Direction::DOWN;
//         }
//         break;

//     case sf::Keyboard::Left:
//         if (dir != Direction::RIGHT) {
//             changedir = Direction::LEFT;
//         }
//         break;

//     case sf::Keyboard::Right:
//         if (dir != Direction::LEFT) {
//             changedir = Direction::RIGHT;
//         }
//         break;

//     default:
//         break;
//     }
// }
