#include "Food.hpp"
#include "Game.hpp"

FoodManager::FoodManager(Game *game, int foodcount) :
    game(game), foodcount(foodcount) {
}

void FoodManager::init() {
    std::cout << foodcount << "\n";
    for (int i = 0; i < foodcount; i++) {
        generateFood(true);
    }
}

void FoodManager::regenerate() {
    foodcount--;
    generateFood();
}

void FoodManager::generateFood(bool init) {
    int x, y;
    int limit = 0;
    int timeout = foodcount > 1 ? 10 : 100;
    do {
        limit++;
        x = game->rand.getX();
        y = game->rand.getY();
    } while (limit <= timeout && game->grid[y][x] != 0);

    if (game->grid[y][x] == 0) {
        game->grid[y][x] = -1;
        if (!init) {
            foodcount++;
        }
    }
}