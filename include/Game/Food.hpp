#ifndef FOOD_MANAGER_HPP
#define FOOD_MANAGER_HPP

// forward declaration
class Game;

/**
 * Handles food generation in game
 */
class FoodManager {
public:
    void generateFood(Game *, int = 1);
};

#endif