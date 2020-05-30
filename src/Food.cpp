#include "Food.hpp"
#include "Game.hpp"
#include <vector>
#include <utility>
#include <random>

FoodManager::FoodManager(Game *game, int foodcount) :
    game(game), foodcount(foodcount) {
}

void FoodManager::init() {
    for (int i = 0; i < foodcount; i++) {
        generateFood(true);
    }
}

void FoodManager::regenerate() {
    foodcount--;
    generateFood();
}

void FoodManager::generateFood(bool init) {
    // int x, y;
    // int limit = 0;
    // int timeout = foodcount > 1 ? 100 : 500;
    std::vector<std::pair<int, int>> empty;
    for(int x = 0; x < Config::COLS; x++) {
        for(int y = 0; y < Config::ROWS; y++) {
            if (game->grid[y][x] == 0) empty.push_back({x, y});
        }
    }
    std::random_device rand;
    std::mt19937 gen(rand());
    std::uniform_int_distribution<int> dis(0, empty.size()-1);
    if (init) {
        for(int i = 0; i < foodcount; i++) {
            int ind = dis(gen);
            game->grid[empty[ind].second][empty[ind].first] = -1;
        }
    }
    else {
        int ind = dis(gen);
        game->grid[empty[ind].second][empty[ind].first] = -1;
    }

    // do {
    //     limit++;
    //     x = game->rand.getX();
    //     y = game->rand.getY();
    // } while (game->grid[y][x] != 0);
    // } while (limit <= timeout && game->grid[y][x] != 0);

    // if (game->grid[y][x] == 0) {
    //     game->grid[y][x] = -1;
    //     if (!init) {
    //         foodcount++;
    //     }
    // }
}