#include "Genetic/Genetic.hpp"

// STATIC MEMBERS AND FUNCTION
std::mt19937 GeneticUtils::gen = std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());
std::normal_distribution<double> GeneticUtils::randn = std::normal_distribution<double>(0.0, 1.0);
std::uniform_real_distribution<double> GeneticUtils::rand = std::uniform_real_distribution<double>(0.0, 1.0);

int GeneticAlgo::generations = Config::GENERATIONS;
int GeneticAlgo::popSize = Config::POPSIZE;
int GeneticAlgo::offSpringSize = Config::NEW_POPSIZE;
double GeneticAlgo::etaX = Config::SBX_ETA;
double GeneticAlgo::mutationProb = Config::MUTATION_RATE;

std::pair<Eigen::MatrixXd, Eigen::MatrixXd> GeneticAlgo::simulatedBinaryCrossover(const Eigen::MatrixXd &w1, const Eigen::MatrixXd &w2) {
    int m = w1.rows();
    int n = w1.cols();

    Eigen::MatrixXd gamma = Eigen::MatrixXd(m, n);
    Eigen::MatrixXd rand = Eigen::MatrixXd(m, n).unaryExpr([&](double dummy) { return GeneticUtils::rand(GeneticUtils::gen); });
    gamma = (rand.array() <= 0.5).select((2 * rand).array().pow(1.0 / (etaX + 1.0)), gamma);
    gamma = (rand.array() > 0.5).select((1.0 / (2.0 * (1.0 - rand.array()))).array().pow(etaX + 1.0), gamma);

    Eigen::MatrixXd child1 = Eigen::MatrixXd(m, n);
    Eigen::MatrixXd child2 = Eigen::MatrixXd(m, n);
    Eigen::MatrixXd oneMinusGamma = 1 - gamma.array();
    Eigen::MatrixXd onePlusGamma = 1 + gamma.array();
    child1 = 0.5 * (onePlusGamma.cwiseProduct(w1) + oneMinusGamma.cwiseProduct(w2));
    child1 = 0.5 * (oneMinusGamma.cwiseProduct(w1) + onePlusGamma.cwiseProduct(w2));

    return {child1, child2};
}

std::pair<Eigen::VectorXd, Eigen::VectorXd> GeneticAlgo::simulatedBinaryCrossover(const Eigen::VectorXd &b1, const Eigen::VectorXd &b2) {
    int m = b1.size();

    Eigen::VectorXd gamma = Eigen::VectorXd(m);
    Eigen::MatrixXd rand = Eigen::VectorXd(m).unaryExpr([&](double dummy) { return GeneticUtils::rand(GeneticUtils::gen); });
    gamma = (rand.array() <= 0.5).select((2 * rand).array().pow(1.0 / (etaX + 1.0)), gamma);
    gamma = (rand.array() > 0.5).select((1.0 / (2.0 * (1.0 - rand.array()))).array().pow(etaX + 1.0), gamma);

    Eigen::VectorXd child1 = Eigen::VectorXd(m);
    Eigen::VectorXd child2 = Eigen::VectorXd(m);
    Eigen::VectorXd oneMinusGamma = 1 - gamma.array();
    Eigen::VectorXd onePlusGamma = 1 + gamma.array();
    child1 = 0.5 * (onePlusGamma.cwiseProduct(b1) + oneMinusGamma.cwiseProduct(b2));
    child2 = 0.5 * (oneMinusGamma.cwiseProduct(b1) + onePlusGamma.cwiseProduct(b2));

    return {child1, child2};
}

void GeneticAlgo::gaussianMutation(Eigen::MatrixXd &w) {
    int m = w.rows();
    int n = w.cols();
    Eigen::MatrixXd rand = Eigen::MatrixXd(m, n).unaryExpr([&](double dummy) { return GeneticUtils::rand(GeneticUtils::gen); });
    Eigen::MatrixXd mutations = Eigen::MatrixXd(m, n).unaryExpr([&](double dummy) { return GeneticUtils::randn(GeneticUtils::gen); }) + w;
    w = (rand.array() < mutationProb).select(mutations, w);

    // for (int i = 0; i < m; i++) {
    //     for (int j = 0; j < n; j++) {
    //         double prob = rand(gen);
    //         if (prob < mutationProb) {
    //             w[i][j] += randn(gen);
    //         }
    //     }
    // }
}

void GeneticAlgo::gaussianMutation(Eigen::VectorXd &b) {
    int m = b.rows();
    Eigen::VectorXd rand = Eigen::VectorXd(m).unaryExpr([&](double dummy) { return GeneticUtils::rand(GeneticUtils::gen); });
    Eigen::VectorXd mutations = Eigen::VectorXd(m).unaryExpr([&](double dummy) { return GeneticUtils::randn(GeneticUtils::gen); }) + b;
    b = (rand.array() < mutationProb).select(mutations, b);

    // int m = b.size();
    // for (int i = 0; i < m; i++) {
    //     double prob = rand(gen);
    //     if (prob < mutationProb) {
    //         b[i] += randn(gen);
    //     }
    // }
}

// // NON-STATIC MEMBERS AND FUNCTIONS

GeneticAlgo::GeneticAlgo() :
    population(std::vector<Individual>(Config::POPSIZE)) {
    currGen = 0;
}

void GeneticAlgo::start() {
    while (++currGen < generations) {
        std::cout << "Generation : " << currGen << "\n";
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

    std::cout << population[0].fitness << std::endl;
    for (int i = 0; i < 10; i++) {
        Render rend(population[i].snake.net.weights, population[i].snake.net.biases);
        rend.start();
    }
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
        double pick = dist(GeneticUtils::gen);
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
        const std::vector<Eigen::MatrixXd> &weights1 = offsprings[i].snake.net.weights;
        const std::vector<Eigen::MatrixXd> &weights2 = offsprings[i + 1].snake.net.weights;
        const std::vector<Eigen::VectorXd> &biases1 = offsprings[i].snake.net.biases;
        const std::vector<Eigen::VectorXd> &biases2 = offsprings[i + 1].snake.net.biases;
        int l = weights1.size();

        std::vector<Eigen::MatrixXd> offWeights1;
        std::vector<Eigen::MatrixXd> offWeights2;
        std::vector<Eigen::VectorXd> offBiases1;
        std::vector<Eigen::VectorXd> offBiases2;

        for (int i = 0; i < l; i++) {
            std::pair<Eigen::MatrixXd, Eigen::MatrixXd> w = simulatedBinaryCrossover(weights1[i], weights2[i]);
            std::pair<Eigen::VectorXd, Eigen::VectorXd> b = simulatedBinaryCrossover(biases1[i], biases2[i]);
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

    for (int i = 0, j = 0, k = 0; k < popSize;) {
        if (oldPopulation[i].fitness > offsprings[j].fitness) {
            population.push_back(oldPopulation[i]);
            i++;
            k++;
        }
        else {
            population.push_back(offsprings[j]);
            j++;
            k++;
        }
    }
}