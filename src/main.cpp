#include "Game/Render.hpp"
#include "Genetic/Genetic.hpp"
#include "Tournament/Tournament.hpp"

int main() {
    // Train a snake
    GeneticAlgo gen;
    gen.start(true, true);

    // Replay best snake
    Render render;
    render.loadFromFile(0);
    render.start();

    // Tournament tourney(render.snake.net.weights, render.snake.net.biases);
    // tourney.start();
}
