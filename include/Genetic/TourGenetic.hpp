#ifndef TOUR_GENETIC_HPP
#define TOUR_GENETIC_HPP

#include "Genetic/Genetic.hpp"
#include "Genetic/Tourney.hpp"

/*
 * GA for tournament training (WIP)
 */
class TourGenetic : public GeneticAlgo {
protected:
    std::vector<Tourney> population;
    std::vector<Tourney> offsprings;
    std::vector<Tourney> globalBest;

    void updateAndLog(int &, bool, bool); // used for updating global best and logging
public:
    TourGenetic();
    void calculateFitness();                    // find fitness of offsprings by running the game
    void elitismSelection();                    // Select the best individuals from offsprings to serve as new population
    void crossoverAndMutation(double totalFit); // Perform roulette selection, crossover and mutation to generate 2 new offsprings
    void nextGeneration();                      // Generate the offsprings for next generation
    void start(bool = true, bool = false);      // start the GA
};

#endif