#ifndef GENETIC_CPP
#define GENETIC_CPP

#include "Genetic/Individual.hpp"
#include "Render.hpp"
#include <Eigen/Dense>
#include <algorithm>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

namespace GeneticUtils {
    extern std::mt19937 gen;
    extern std::normal_distribution<double> randn;
    extern std::uniform_real_distribution<double> rand;
}; // namespace GeneticUtils

class GeneticAlgo {
private:
    static int generations;
    static int popSize;
    static int offSpringSize;
    static double etaX;
    static double mutationProb;

    static std::pair<Eigen::MatrixXd, Eigen::MatrixXd> simulatedBinaryCrossover(const Eigen::MatrixXd &, const Eigen::MatrixXd &);
    static std::pair<Eigen::VectorXd, Eigen::VectorXd> simulatedBinaryCrossover(const Eigen::VectorXd &, const Eigen::VectorXd &);
    static void gaussianMutation(Eigen::MatrixXd &);
    static void gaussianMutation(Eigen::VectorXd &);

    int currGen;
    std::vector<Individual> population;
    std::vector<Individual> offsprings;

public:
    GeneticAlgo();
    void start();
    void calculateFitness(std::vector<Individual> &);
    void parentSelection();
    void crossoverAndMutation();
    void populationSelection();
    std::vector<Individual> selection();
};

#endif