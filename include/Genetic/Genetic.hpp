#ifndef GENETIC_CPP
#define GENETIC_CPP

#include "Game/Render.hpp"
#include "Genetic/Individual.hpp"
#include <Eigen/Dense>
#include <algorithm>
#include <random>
#include <utility>
#include <vector>

namespace GeneticUtils {
    // RNGs
    extern std::mt19937 gen;
    extern std::normal_distribution<double> randn;
    extern std::uniform_real_distribution<double> rand;
}; // namespace GeneticUtils

class GeneticAlgo {
protected:
    static int generations;
    static int popSize;
    static int offspringsSize;
    static double etaX;
    static double mutationProb;

    // Genetic operators
    // CROSSOVER
    static std::pair<Eigen::MatrixXd, Eigen::MatrixXd> simulatedBinaryCrossover(const Eigen::MatrixXd &, const Eigen::MatrixXd &);
    static std::pair<Eigen::VectorXd, Eigen::VectorXd> simulatedBinaryCrossover(const Eigen::VectorXd &, const Eigen::VectorXd &);
    static std::pair<Eigen::MatrixXd, Eigen::MatrixXd> singlePointCrossover(const Eigen::MatrixXd &, const Eigen::MatrixXd &);
    static std::pair<Eigen::VectorXd, Eigen::VectorXd> singlePointCrossover(const Eigen::VectorXd &, const Eigen::VectorXd &);

    // MUTATION
    static void gaussianMutation(Eigen::MatrixXd &);
    static void gaussianMutation(Eigen::VectorXd &);

    int currentGen;                     // current generation number
    std::vector<Individual> population; // current population
    std::vector<Individual> offsprings; // new offsprings
    std::vector<Individual> globalBest; // best individuals

    void updateAndLog(int &, bool, bool); // used for updating global best and logging

public:
    GeneticAlgo();
    void calculateFitness();                    // find fitness of offsprings by running the game
    void elitismSelection();                    // Select the best individuals from offsprings to serve as new population
    void crossoverAndMutation(double totalFit); // Perform roulette selection, crossover and mutation to generate 2 new offsprings
    void nextGeneration();                      // Generate the offsprings for next generation
    void start(bool = true, bool = false);      // start the GA
};

#endif