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

    static std::mt19937 gen;
    static std::normal_distribution<double> randn;
    static std::uniform_real_distribution<double> rand;
    static int generations;
    static int popSize;
    static int offSpringSize;
    static double etaX;
    static double mutationProb;

    static std::pair<VecWeights, VecWeights> simulatedBinaryCrossover(const VecWeights &, const VecWeights &);
    static std::pair<VecBiases, VecBiases> simulatedBinaryCrossover(const VecBiases &, const VecBiases &);
    static void gaussianMutation(VecWeights &);
    static void gaussianMutation(VecBiases &);

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