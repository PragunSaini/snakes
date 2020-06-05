#include "Genetic/TourGenetic.hpp"
#include "Config.hpp"
#include "Tournament/Tournament.hpp"
#include <iostream>

TourGenetic::TourGenetic() :
    GeneticAlgo(),
    offsprings(popSize) {
    GeneticAlgo::offsprings.clear();
}

// Simulate the population playing games
void TourGenetic::calculateFitness() {
    for (Tourney &tourney : offsprings) {
        tourney.start();
    }
    std::sort(offsprings.begin(), offsprings.end(), std::greater<Tourney>());
}

void TourGenetic::elitismSelection() {
    population.clear();
    population.reserve(popSize);
    for (int i = 0; i < popSize; i++) {
        population.push_back(offsprings[i]);
    }
}

void TourGenetic::crossoverAndMutation(double totalFit) {
    // Select two parents from population
    // Using Roulette selection
    int par1 = 0, par2 = 0;
    std::uniform_real_distribution<double> dist(0, totalFit);
    for (int i = 0; i < 2; i++) {
        double pick = dist(GeneticUtils::gen);
        double current = 0;
        for (int j = 0; j < popSize; j++) {
            current += population[j].fitness;
            if (current >= pick) {
                if (i == 0) {
                    par1 = j;
                }
                else {
                    par2 = j;
                }
                break;
            }
        }
    }

    Tourney &parent1 = population[par1];
    Tourney &parent2 = population[par2];

    Tourney child1;
    Tourney child2;

    for (int p = 0; p < Config::PLAYERCNT; p++) {

        // Get their weights and biases
        const std::vector<Eigen::MatrixXd> &weights1 = parent1.players[p].net.weights;
        const std::vector<Eigen::MatrixXd> &weights2 = parent2.players[p].net.weights;
        const std::vector<Eigen::VectorXd> &biases1 = parent1.players[p].net.biases;
        const std::vector<Eigen::VectorXd> &biases2 = parent2.players[p].net.biases;
        int l = weights1.size();

        std::vector<Eigen::MatrixXd> offWeights1;
        std::vector<Eigen::MatrixXd> offWeights2;
        std::vector<Eigen::VectorXd> offBiases1;
        std::vector<Eigen::VectorXd> offBiases2;

        // Perform Crossover and Mutation to generate two children
        for (int i = 0; i < l; i++) {
            double SBXOrSPC = GeneticUtils::rand(GeneticUtils::gen);
            std::pair<Eigen::MatrixXd, Eigen::MatrixXd> w = SBXOrSPC <= 0.5 ? simulatedBinaryCrossover(weights1[i], weights2[i]) : singlePointCrossover(weights1[i], weights2[i]);
            std::pair<Eigen::VectorXd, Eigen::VectorXd> b = SBXOrSPC <= 0.5 ? simulatedBinaryCrossover(biases1[i], biases2[i]) : singlePointCrossover(biases1[i], biases2[i]);
            gaussianMutation(w.first);
            gaussianMutation(w.second);
            gaussianMutation(b.first);
            gaussianMutation(b.second);
            offWeights1.push_back(w.first);
            offWeights2.push_back(w.second);
            offBiases1.push_back(b.first);
            offBiases2.push_back(b.second);
        }

        child1.players[p].net.weights = offWeights1;
        child1.players[p].net.biases = offBiases1;
        child2.players[p].net.weights = offWeights2;
        child2.players[p].net.biases = offBiases2;
    }
    // Add to offsprings
    offsprings.push_back(child1);
    offsprings.push_back(child2);
}

void TourGenetic::nextGeneration() {
    offsprings.clear();
    offsprings.reserve(offspringsSize);

    // Carry forward current population to next generation as is
    for (Tourney &tourney : population) {
        offsprings.push_back(tourney);
        offsprings.back().reseed(); // important, otherwise parents will replay performance
    }

    // Find total fitness of current population (for roulette selection)
    double totFitness = 0.0;
    for (int i = 0; i < popSize; i++) {
        totFitness += population[i].fitness;
    }

    // Generate offsprings
    int k = offsprings.size();
    while (k < offspringsSize) {
        k += 2;
        crossoverAndMutation(totFitness);
    }
}

void TourGenetic::updateAndLog(int &lastImprov, bool log, bool saveBestPerGeneration) {
    // Update current best (if new best found or maximum score achieved)
    if (globalBest.empty() || globalBest[0].fitness < population[0].fitness) {
        // Reset mutation rate
        mutationProb = Config::MUTATION_RATE;
        lastImprov = 0;

        // Save best 10 of the population
        globalBest.clear();
        for (int i = 0; i < 2; i++) {
            globalBest.push_back(population[i]);
        }
    }

    // If no improvement for a long time, increase mutation rate
    if (lastImprov > 0 && lastImprov % 10 == 0) {
        mutationProb *= 1.025;
        if (mutationProb > 0.4)
            mutationProb = 0.15;
    }

    if (log) {
        // Print stats, every 10th generation
        if (currentGen % 10 == 0) {
            std::cout << "----------------------"
                      << "\n";
            std::cout << "Generation : " << currentGen << "\n";
            std::cout << "Best Fitness : " << globalBest[0].fitness << "\n";
            std::cout << "Best Score : " << globalBest[0].score << "\n";
            std::cout << "Last Improv : " << lastImprov << "\n";
        }
    }

    if (saveBestPerGeneration) {
        globalBest[0].saveToFile();
    }
}

void TourGenetic::start(bool log, bool saveBestPerGeneration) {
    // track improvements
    int lastImprov = 0;

    while (++currentGen < generations) {
        lastImprov++;

        // First let the current population calculate fitness
        calculateFitness();

        // Then perform selection of parents
        elitismSelection();

        // Update best and log
        updateAndLog(lastImprov, log, saveBestPerGeneration);

        // Upon maximum score, check if wanna exit ?
        if (currentGen >= 50 && currentGen % 50 == 0) {
            std::cout << "Best snake achieved, want to continue ?";
            char ch;
            std::cin >> ch;
            if (ch != 'Y' && ch != 'y')
                break;
        }

        // Select best for new population
        nextGeneration();
    }

    // Print results
    if (log) {
        for (int i = 0; i < 2; i++)
            std::cout << globalBest[i].fitness << " | " << globalBest[i].score << "\n";
    }

    // Save the best snake with file offset 0
    globalBest[0].saveToFile();

    std::cout << "Training Complete !!"
              << "\n";
}
