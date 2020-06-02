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

    Eigen::MatrixXd rand = Eigen::MatrixXd(m, n).unaryExpr([&](__attribute__((unused)) double dummy) { return GeneticUtils::rand(GeneticUtils::gen); });
    Eigen::MatrixXd gamma = (rand.array() <= 0.5).select((2 * rand).array().pow(1.0 / (etaX + 1.0)), (1.0 / (2.0 * (1.0 - rand.array()))).array().pow(1.0 / (etaX + 1.0)));

    Eigen::MatrixXd oneMinusGamma = 1 - gamma.array();
    Eigen::MatrixXd onePlusGamma = 1 + gamma.array();
    Eigen::MatrixXd child1 = 0.5 * (onePlusGamma.cwiseProduct(w1) + oneMinusGamma.cwiseProduct(w2));
    Eigen::MatrixXd child2 = 0.5 * (oneMinusGamma.cwiseProduct(w1) + onePlusGamma.cwiseProduct(w2));

    return {child1, child2};
}

std::pair<Eigen::VectorXd, Eigen::VectorXd> GeneticAlgo::simulatedBinaryCrossover(const Eigen::VectorXd &b1, const Eigen::VectorXd &b2) {
    int m = b1.size();

    Eigen::MatrixXd rand = Eigen::VectorXd(m).unaryExpr([&](__attribute__((unused)) double dummy) { return GeneticUtils::rand(GeneticUtils::gen); });
    Eigen::VectorXd gamma = (rand.array() <= 0.5).select((2 * rand).array().pow(1.0 / (etaX + 1.0)), (1.0 / (2.0 * (1.0 - rand.array()))).array().pow(1.0 / (etaX + 1.0)));

    Eigen::VectorXd oneMinusGamma = 1 - gamma.array();
    Eigen::VectorXd onePlusGamma = 1 + gamma.array();
    Eigen::VectorXd child1 = 0.5 * (onePlusGamma.cwiseProduct(b1) + oneMinusGamma.cwiseProduct(b2));
    Eigen::VectorXd child2 = 0.5 * (oneMinusGamma.cwiseProduct(b1) + onePlusGamma.cwiseProduct(b2));

    return {child1, child2};
}

std::pair<Eigen::MatrixXd, Eigen::MatrixXd> GeneticAlgo::singlePointCrossover(const Eigen::MatrixXd &w1, const Eigen::MatrixXd &w2) {
    int rows = w1.rows();
    int cols = w1.cols();
    int r = std::uniform_int_distribution<int>(0, rows - 1)(GeneticUtils::gen);
    int c = std::uniform_int_distribution<int>(0, cols - 1)(GeneticUtils::gen);

    Eigen::MatrixXd child1 = w1;
    Eigen::MatrixXd child2 = w2;
    child1.topRows(r + 1).swap(child2.topRows(r + 1));
    child1.block(r, c, 1, cols - c).swap(child2.block(r, c, 1, cols - c));
    return {child1, child2};
}

std::pair<Eigen::VectorXd, Eigen::VectorXd> GeneticAlgo::singlePointCrossover(const Eigen::VectorXd &b1, const Eigen::VectorXd &b2) {
    int rows = b1.rows();
    int r = std::uniform_int_distribution<int>(0, rows - 1)(GeneticUtils::gen);
    Eigen::VectorXd child1 = b1;
    Eigen::VectorXd child2 = b2;
    child1.topRows(r + 1).swap(child2.topRows(r + 1));
    return {child1, child2};
}

void GeneticAlgo::gaussianMutation(Eigen::MatrixXd &w) {
    int m = w.rows();
    int n = w.cols();
    Eigen::MatrixXd rand = Eigen::MatrixXd(m, n).unaryExpr([&](__attribute__((unused)) double dummy) { return GeneticUtils::rand(GeneticUtils::gen); });
    Eigen::MatrixXd mutations = Eigen::MatrixXd(m, n).unaryExpr([&](__attribute__((unused)) double dummy) { return GeneticUtils::randn(GeneticUtils::gen); }) + w;
    w = (rand.array() < mutationProb).select(mutations, w);
}

void GeneticAlgo::gaussianMutation(Eigen::VectorXd &b) {
    int m = b.rows();
    Eigen::VectorXd rand = Eigen::VectorXd(m).unaryExpr([&](__attribute__((unused)) double dummy) { return GeneticUtils::rand(GeneticUtils::gen); });
    Eigen::VectorXd mutations = Eigen::VectorXd(m).unaryExpr([&](__attribute__((unused)) double dummy) { return GeneticUtils::randn(GeneticUtils::gen); }) + b;
    b = (rand.array() < mutationProb).select(mutations, b);
}

// // NON-STATIC MEMBERS AND FUNCTIONS
GeneticAlgo::GeneticAlgo() :
    offsprings(std::vector<Individual>(Config::POPSIZE)) {
    currGen = 0;
}

void GeneticAlgo::start() {
    // for (int i = 0; i < 100; i++) {
    //     offsprings[i].snake.net.loadFromFile(i%2);
    // }
    int lastImprov = 0;

    while (++currGen < generations) {
        lastImprov++;
        // First let the current population calculate fitness
        calculateFitness();

        // Then perform selection of parents
        elitismSelection();

        // Update current best
        if (globalBest.empty() || globalBest[0].fitness < population[0].fitness) {
            mutationProb = Config::MUTATION_RATE;
            lastImprov = 0;
            globalBest.clear();
            globalBest.insert(globalBest.end(), population.begin(), population.begin() + 150);
        }
        if (lastImprov % 500 == 0) {mutationProb *= 1.025; if (mutationProb > 0.3) mutationProb = 0.3;} 

        if (lastImprov > 15000) break;

        // Print stats
        if (currGen % 100 == 0) {
            std::cout << "----------------------" << "\n";
            std::cout << "Generation : " << currGen << "\n";
            // std::cout << "Round Best Fitness : " << population[0].fitness << "\n";
            std::cout << "Best Fitness : " << globalBest[0].fitness << "\n";
            std::cout << "Second Best Fitness : " << globalBest[1].fitness << "\n";
            std::cout << "Last Improv : " << lastImprov << "\n";
        }

        // Select best for new population
        nextGeneration();
    }

    // Print and store results
    for (int i = 0; i < 150; i++) {
        std::cout << globalBest[i].fitness << "\n";
        globalBest[i].snake.net.saveToFile(i);
        if (i < 10) {
            // Render rend(globalBest[i].snake.net.weights, globalBest[i].snake.net.biases);
            // rend.start();
        }
    }
}

// Simulate the population playing games
void GeneticAlgo::calculateFitness() {
    for (Individual &indiv : offsprings) {
        indiv.start();
    }
    std::sort(offsprings.begin(), offsprings.end(), std::greater<Individual>());
}

void GeneticAlgo::elitismSelection() {
    population.clear();
    population.reserve(popSize);
    for (int i = 0; i < popSize; i++) {
        population.push_back(offsprings[i]);
    }
}

// Perform crossover and mutation to generate child population
void GeneticAlgo::crossoverAndMutation(double totalFit) {
    int par1 = 0, par2 = 0;

    std::uniform_real_distribution<double> dist(0, totalFit);
    for (int i = 0; i < 2; i++) {
        double pick = dist(GeneticUtils::gen);
        double current = 0;
        for (int j = 0; j < popSize; j++) { // popSize, 150
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

    const std::vector<Eigen::MatrixXd> &weights1 = population[par1].snake.net.weights;
    const std::vector<Eigen::MatrixXd> &weights2 = population[par2].snake.net.weights;
    const std::vector<Eigen::VectorXd> &biases1 = population[par1].snake.net.biases;
    const std::vector<Eigen::VectorXd> &biases2 = population[par2].snake.net.biases;
    int l = weights1.size();

    std::vector<Eigen::MatrixXd> offWeights1;
    std::vector<Eigen::MatrixXd> offWeights2;
    std::vector<Eigen::VectorXd> offBiases1;
    std::vector<Eigen::VectorXd> offBiases2;

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
    offsprings.push_back(Individual(offWeights1, offBiases1));
    offsprings.push_back(Individual(offWeights2, offBiases2));
}

void GeneticAlgo::nextGeneration() {
    offsprings.clear();
    offsprings.reserve(offSpringSize);

    for (Individual &indiv : population) {
        offsprings.push_back(indiv);
    }
    double totFitness = 0.0;
    for(int i = 0; i < popSize; i++) {// popSize, 150
        totFitness += population[i].fitness;
    }

    int k = offsprings.size();
    while (k < offSpringSize) {
        k += 2;
        crossoverAndMutation(totFitness);
    }
}