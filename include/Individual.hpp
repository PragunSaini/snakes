#ifndef INDIVIDUAL_HPP
#define INDIVIDUAL_HPP

#include "Config.hpp"
#include "Food.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "Random.hpp"
#include "Snake.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>

class Individual : public Game {
private:
    // void init(const std::vector<NeuralNet::VecWeights> &w = {}, const std::vector<NeuralNet::VecBiases> &b = {});

public:
    double fitness;

    Individual();
    Individual(const std::vector<NeuralNet::VecWeights> &w, const std::vector<NeuralNet::VecBiases> &b);
    Individual(const Individual &);
    Individual &operator=(const Individual &);
    bool operator>(const Individual &) const;
    void start();
};

#endif