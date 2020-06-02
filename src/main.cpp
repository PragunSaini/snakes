// #include "Genetic/Genetic.hpp"
#include "Config.hpp"
#include "Genetic/Genetic.hpp"
#include "NN/NeuralNet.hpp"
#include "Render.hpp"
#include <iostream>

int main() {
    // GeneticAlgo gen;
    // gen.start();

    Render rend;
    rend.snake.net.loadFromFile(0);
    rend.start();
}
