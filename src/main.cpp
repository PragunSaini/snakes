#include "Game/Render.hpp"
#include "Genetic/Genetic.hpp"
#include "Tournament/Tournament.hpp"

int main() {
    // Train a snake
    // GeneticAlgo gen;
    // gen.start(true, false);

    // Manual playing
    // Render manual(false);
    // manual.start();

    // Replay best snake
    // Render trained;
    // trained.loadFromFile(0);
    // trained.start();

    // Play a tournament
    Tournament t;
    t.loadFromFile();
    t.start();
}
