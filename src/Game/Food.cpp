#include "Game/Food.hpp"
#include "Config.hpp"
#include "Game/Game.hpp"
#include <random>
#include <utility>
#include <vector>

void FoodManager::generateFood(Game *game, int count) {
    if (count == 0)
        return;

    std::vector<std::pair<int, int>> empty;
    for (int x = 0; x < Config::COLS; x++) {
        for (int y = 0; y < Config::ROWS; y++) {
            if (game->grid[y][x] == 0)
                empty.push_back({x, y});
        }
    }

    int N = empty.size();
    if (N == 0)
        return;

    for (int i = 0; i < count; i++) {
        if (N == 0)
            break;
        std::uniform_int_distribution<int> dis(0, empty.size() - 1);
        int ind;
        do {
            ind = dis(game->getGen());
        } while (game->grid[empty[ind].second][empty[ind].first] != 0);
        game->grid[empty[ind].second][empty[ind].first] = -1;
        N -= 1;
    }
}