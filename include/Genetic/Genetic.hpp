#ifndef GENETIC_CPP
#define GENETIC_CPP

#include "Genetic/Individual.hpp"
#include "Render.hpp"
#include <algorithm>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

class GeneticAlgo {
private:
    typedef std::vector<double> VecBiases;
    typedef std::vector<std::vector<double>> VecWeights;

    std::mt19937 gen;
    std::normal_distribution<double> randn;
    std::uniform_real_distribution<double> rand;

    int generations;
    int currGen;
    int popsize;
    int newpopsize;
    double eta;
    double mut_prob;

    std::vector<Individual> games;

    std::pair<VecWeights, VecWeights> simulatedBinaryCrossover(const VecWeights &, const VecWeights &);
    std::pair<VecBiases, VecBiases> simulatedBinaryCrossover(const VecBiases &, const VecBiases &);
    void gaussianMutation(VecWeights &);
    void gaussianMutation(VecBiases &);

public:
    GeneticAlgo();
    void start();
    void playgames();
    std::vector<Individual> selection();
    void crossoverAndMutation();
};

#endif