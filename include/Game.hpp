#ifndef GAME_HPP
#define GAME_HPP

#include "Config.hpp"
#include "Food.hpp"
#include "Map.hpp"
#include "Random.hpp"
#include "Snake.hpp"
#include <iostream>
#include <vector>

class Game {
protected:
    void init(const std::vector<NeuralNet::VecWeights> &w = {}, const std::vector<NeuralNet::VecBiases> &b = {});

public:
    std::vector<std::vector<int>> grid;
    FoodManager foodManager;
    Snake snake;
    Random rand;

    Game();
    Game(const std::vector<NeuralNet::VecWeights> &w, const std::vector<NeuralNet::VecBiases> &b);
    Game(const Game &);
    Game &operator=(const Game &);
    virtual void start() = 0;
};

#endif