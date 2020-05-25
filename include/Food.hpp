#ifndef FOOD_MANAGER_HPP
#define FOOD_MANAGER_HPP

#include "Config.hpp"

// forward declaration
class Game;

class FoodManager {
public:
    Game *game;
    int foodcount;

    FoodManager(Game *, int = Config::FOODCNT);
    void generateFood(bool = false);
    void init();
    void regenerate();
};

#endif