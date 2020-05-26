#include "Snake.hpp"
#include "Game.hpp"

// Initialize a snake body part at given coords
Snakebody::Snakebody(int x, int y) :
    x(x), y(y), newblock(true) {}

Snake::Snake(Game *game, int color) :
    game(game), color(color), alive(true), net({32, 20, 12, 4}) {
}

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

void Snake::init(int x, int y) {
    // Initialize a snake with one body part
    length = 1;
    snake.push_back(Snakebody(x, y));
    snake[0].newblock = false;
    game->grid[y][x] = color;
    // dir = Direction::DOWN;
    snake[0].dir = (Direction)(game->rand.getX() % 4);
    score = 0;
    steps = 0;
    stepsSinceFood = 0;
}

// bool Snake::willHitWall() {
//     switch (snake[0].dir) {
//     case Direction::UP:
//         if (snake[0].y == 0)
//             return true;
//         break;

//     case Direction::DOWN:
//         if (snake[0].y == Config::ROWS - 1)
//             return true;
//         break;

//     case Direction::LEFT:
//         if (snake[0].x == 0)
//             return true;
//         break;

//     case Direction::RIGHT:
//         if (snake[0].x == Config::COLS - 1)
//             return true;
//         break;

//     default:
//         return false;
//     }
//     return false;
// }

void Snake::moveBody() {
    if (!snake[length - 1].newblock)
        game->grid[snake[length - 1].y][snake[length - 1].x] = 0;

    for (int i = length - 1; i >= 1; i--) {
        snake[i] = snake[i - 1];
        game->grid[snake[i].y][snake[i].x] = color;
    }
}

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

void Snake::increaseLength() {
    snake.push_back(Snakebody(0, 0));
    snake[length].dir = snake[length - 1].dir;
    length++;
}

void Snake::update() {
    if (alive) {
        stepsSinceFood++;

        std::vector<double> res = net.feedforward(getVision());
        int dist = std::distance(res.begin(), std::max_element(res.begin(), res.end()));
        changedir = (Direction)dist;

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
            game->grid[snake[0].y][snake[0].x] = color;
            game->foodManager.regenerate();
        }
        else {
            game->grid[snake[0].y][snake[0].x] = color;
        }

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
    std::cout << "Score : " << score << " | Steps : " << steps << "\n";
}

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

bool Snake::inRange(int x, int y) {
    if (x >= 0 && y >= 0 && y < Config::ROWS && x < Config::COLS)
        return true;
    return false;
}

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