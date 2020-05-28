#include "Genetic/Genetic.hpp"

// STATIC MEMBERS AND FUNCTION
std::mt19937 GeneticAlgo::gen = std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());
std::normal_distribution<double> GeneticAlgo::randn = std::normal_distribution<double>(0.0, 1.0);
std::uniform_real_distribution<double> GeneticAlgo::rand = std::uniform_real_distribution<double>(0.0, 1.0);

int GeneticAlgo::generations = Config::GENERATIONS;
int GeneticAlgo::popSize = Config::POPSIZE;
int GeneticAlgo::offSpringSize = Config::NEW_POPSIZE;
double GeneticAlgo::etaX = Config::SBX_ETA;
double GeneticAlgo::mutationProb = Config::MUTATION_RATE;

std::pair<GeneticAlgo::VecWeights, GeneticAlgo::VecWeights> GeneticAlgo::simulatedBinaryCrossover(const VecWeights &w1, const VecWeights &w2) {
    int m = w1.size();
    int n = w1[0].size();
    VecWeights gamma = VecWeights(m, std::vector<double>(n));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            double r = rand(gen);
            if (r <= 0.5) {
                gamma[i][j] = std::pow(2 * r, 1.0 / (etaX + 1));
            }
            else {
                gamma[i][j] = std::pow(1.0 / (2.0 * (1.0 - r)), 1.0 / (etaX + 1));
            }
        }
    }

    VecWeights child1 = VecWeights(m, std::vector<double>(n));
    VecWeights child2 = VecWeights(m, std::vector<double>(n));
#pragma omp simd
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            child1[i][j] = 0.5 * ((1 + gamma[i][j]) * w1[i][j] + (1 - gamma[i][j]) * w2[i][j]);
            child2[i][j] = 0.5 * ((1 - gamma[i][j]) * w1[i][j] + (1 + gamma[i][j]) * w2[i][j]);
        }
    }
    return {child1, child2};
}

std::pair<GeneticAlgo::VecBiases, GeneticAlgo::VecBiases> GeneticAlgo::simulatedBinaryCrossover(const VecBiases &b1, const VecBiases &b2) {
    int m = b1.size();
    VecBiases gamma = VecBiases(m);
    for (int i = 0; i < m; i++) {
        double r = rand(gen);
        if (r <= 0.5) {
            gamma[i] = std::pow(2 * r, 1.0 / (etaX + 1));
        }
        else {
            gamma[i] = std::pow(1.0 / (2.0 * (1.0 - r)), 1.0 / (etaX + 1));
        }
    }

    VecBiases child1 = VecBiases(m);
    VecBiases child2 = VecBiases(m);
#pragma omp simd
    for (int i = 0; i < m; i++) {
        child1[i] = 0.5 * ((1 + gamma[i]) * b1[i] + (1 - gamma[i]) * b2[i]);
        child2[i] = 0.5 * ((1 - gamma[i]) * b1[i] + (1 + gamma[i]) * b2[i]);
    }
    return {child1, child2};
}

void GeneticAlgo::gaussianMutation(VecWeights &w) {
    int m = w.size();
    int n = w[0].size();
#pragma omp simd
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            double prob = rand(gen);
            if (prob < mutationProb) {
                w[i][j] += randn(gen);
            }
        }
    }
}

void GeneticAlgo::gaussianMutation(VecBiases &b) {
    int m = b.size();
#pragma omp simd
    for (int i = 0; i < m; i++) {
        double prob = rand(gen);
        if (prob < mutationProb) {
            b[i] += randn(gen);
        }
    }
}

// NON-STATIC MEMBERS AND FUNCTIONS

GeneticAlgo::GeneticAlgo() :
    population(std::vector<Individual>(Config::POPSIZE)) {
    currGen = 0;
}

void GeneticAlgo::start() {
    while (++currGen < generations) {
        std::cout << "Generation : " << currGen << std::endl;
        // First let the current population calculate fitness
        calculateFitness(population);

        // Then perform selection of parents
        parentSelection();

        // Generate offsprings using crossover and mutation
        crossoverAndMutation();

        // Calculate offsprings fitness
        calculateFitness(offsprings);

        // Select best for new population
        populationSelection();

        // if (currGen % 100 == 0) {
        //     std::cout << population[0].fitness << std::endl;
        //     for (int i = 0; i < 10; i++) {
        //         Render rend(population[i].snake.net.weights, population[i].snake.net.biases);
        //         rend.start();
        //     }
        // }
    }

    // std::cout << population[0].fitness << std::endl;
    // for (int i = 0; i < 10; i++) {
    //     Render rend(population[i].snake.net.weights, population[i].snake.net.biases);
    //     rend.start();
    // }
}

// Simulate the population playing games
void GeneticAlgo::calculateFitness(std::vector<Individual> &population) {
    for (Individual &indiv : population) {
        indiv.start();
    }
    std::sort(population.begin(), population.end(), std::greater<Individual>());
}

void GeneticAlgo::parentSelection() {
    offsprings.clear();
    offsprings.reserve(offSpringSize);

    double totFitness = 0.0;
    for (Individual &indiv : population) {
        totFitness += indiv.fitness;
    }

    std::uniform_real_distribution<double> dist(0, totFitness);
    for (int i = 0; i < offSpringSize; i++) {
        double pick = dist(gen);
        double current = 0;
        for (Individual &indiv : population) {
            current += indiv.fitness;
            if (current >= pick) {
                offsprings.push_back(indiv);
                break;
            }
        }
    }
}

// Perform crossover and mutation to generate child population
void GeneticAlgo::crossoverAndMutation() {
    for (int i = 0; i < offSpringSize; i += 2) {
        const std::vector<VecWeights> &weights1 = offsprings[i].snake.net.weights;
        const std::vector<VecWeights> &weights2 = offsprings[i + 1].snake.net.weights;
        const std::vector<VecBiases> &biases1 = offsprings[i].snake.net.biases;
        const std::vector<VecBiases> &biases2 = offsprings[i + 1].snake.net.biases;
        int l = weights1.size();

        std::vector<VecWeights> offWeights1;
        std::vector<VecWeights> offWeights2;
        std::vector<VecBiases> offBiases1;
        std::vector<VecBiases> offBiases2;

        for (int i = 0; i < l; i++) {
            std::pair<VecWeights, VecWeights> w = simulatedBinaryCrossover(weights1[i], weights2[i]);
            std::pair<VecBiases, VecBiases> b = simulatedBinaryCrossover(biases1[i], biases2[i]);
            gaussianMutation(w.first);
            gaussianMutation(w.second);
            gaussianMutation(b.first);
            gaussianMutation(b.second);
            offWeights1.push_back(w.first);
            offWeights2.push_back(w.second);
            offBiases1.push_back(b.first);
            offBiases2.push_back(b.second);
        }

        offsprings[i] = Individual(offWeights1, offBiases1);
        offsprings[i + 1] = Individual(offWeights2, offBiases2);
    }
}

void GeneticAlgo::populationSelection() {
    std::vector<Individual> oldPopulation = population;
    population.clear();
    population.reserve(popSize);

    for (int i = 0, j = 0; population.size() < popSize;) {
        if (oldPopulation[i].fitness > offsprings[j].fitness) {
            population.push_back(oldPopulation[i]);
            i++;
        }
        else {
            population.push_back(offsprings[j]);
            j++;
        }
    }
}