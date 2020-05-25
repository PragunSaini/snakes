#ifndef FOOD_MANAGER_HPP
#define FOOD_MANAGER_HPP

// forward declaration
class Game;

class FoodManager {
public:
    Game *game;
    int foodcount;

    FoodManager(Game *, int = 50);
    void generateFood(bool = false);
    void init();
    void regenerate();
};

#endif