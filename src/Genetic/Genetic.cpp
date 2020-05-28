#include "Genetic/Genetic.hpp"
using std::cout;
using std::endl;

GeneticAlgo::GeneticAlgo() :
    gen(std::mt19937(std::chrono::system_clock::now().time_since_epoch().count())),
    randn(std::normal_distribution<double>(0.0, 1.0)),
    rand(std::uniform_real_distribution<double>(0.0, 1.0)),
    games(std::vector<Individual>(500)) {
    generations = 100;
    popsize = 500;
    newpopsize = Config::NEW_POPSIZE;
    eta = Config::SBX_ETA;
    mut_prob = Config::MUTATION_RATE;
    currGen = 0;
}

void GeneticAlgo::start() {
    while (currGen++ < generations) {
        std::cout << "GENERATION : " << currGen << "\n";
        playgames();
        games = selection();
        // break;
        crossoverAndMutation();
    }
    playgames();
    for (Individual &g : games)
        std::cout << g.fitness << "| " << g.snake.score << " |" << g.snake.steps << "\n";
}

// Simulate the population playing games
void GeneticAlgo::playgames() {
    for (Individual &game : games) {
        game.start();
    }
    std::sort(games.begin(), games.end(), std::greater<Individual>());
}

// Select best chromosomes using roulette selection
std::vector<Individual> GeneticAlgo::selection() {
    std::vector<Individual> selection;
    selection.reserve(popsize);
    double totFitness = 0.0;
    for (Individual &game : games) {
        totFitness += game.fitness;
    }
    std::uniform_real_distribution<double> dist(0, totFitness);
    for (int i = 0; i < popsize; i++) {
        double pick = dist(gen);
        double current = 0;
        for (Individual &game : games) {
            current += game.fitness;
            if (current >= pick) {
                selection.push_back(game);
                if (currGen == 100) {
                    std::cout << selection[i].fitness << std::endl;
                }
                break;
            }
        }
    }
    return selection;
}

std::pair<GeneticAlgo::VecWeights, GeneticAlgo::VecWeights> GeneticAlgo::simulatedBinaryCrossover(const VecWeights &w1, const VecWeights &w2) {
    int m = w1.size();
    int n = w1[0].size();
    VecWeights gamma = VecWeights(m, std::vector<double>(n));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            double r = rand(gen);
            if (r <= 0.5) {
                gamma[i][j] = std::pow(2 * r, 1.0 / (eta + 1));
            }
            else {
                gamma[i][j] = std::pow(1.0 / (2.0 * (1.0 - r)), 1.0 / (eta + 1));
            }
        }
    }

    VecWeights child1 = VecWeights(m, std::vector<double>(n));
    VecWeights child2 = VecWeights(m, std::vector<double>(n));
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
            gamma[i] = std::pow(2 * r, 1.0 / (eta + 1));
        }
        else {
            gamma[i] = std::pow(1.0 / (2.0 * (1.0 - r)), 1.0 / (eta + 1));
        }
    }

    VecBiases child1 = VecBiases(m);
    VecBiases child2 = VecBiases(m);
    for (int i = 0; i < m; i++) {
        child1[i] = 0.5 * ((1 + gamma[i]) * b1[i] + (1 - gamma[i]) * b2[i]);
        child2[i] = 0.5 * ((1 - gamma[i]) * b1[i] + (1 + gamma[i]) * b2[i]);
    }
    return {child1, child2};
}

void GeneticAlgo::gaussianMutation(VecWeights &w) {
    int m = w.size();
    int n = w[0].size();
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            double prob = rand(gen);
            if (prob < mut_prob) {
                w[i][j] += randn(gen);
            }
        }
    }
}

void GeneticAlgo::gaussianMutation(VecBiases &b) {
    int m = b.size();
    for (int i = 0; i < m; i++) {
        double prob = rand(gen);
        if (prob < mut_prob) {
            b[i] += randn(gen);
        }
    }
}

// Perform crossover and mutation to generate child population
void GeneticAlgo::crossoverAndMutation() {
    std::vector<Individual> offsprings(popsize);
    for (int i = 0; i < popsize; i += 2) {
        const std::vector<VecWeights> &weights1 = games[i].snake.net.weights;
        const std::vector<VecWeights> &weights2 = games[i + 1].snake.net.weights;
        const std::vector<VecBiases> &biases1 = games[i].snake.net.biases;
        const std::vector<VecBiases> &biases2 = games[i + 1].snake.net.biases;
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

        offsprings.push_back(Individual(offWeights1, offBiases1));
        offsprings.push_back(Individual(offWeights2, offBiases2));
    }

    games = offsprings;
}
