#ifndef NEURAL_NET_HPP
#define NEURAL_NET_HPP

#include <Eigen/Dense>
#include <random>
#include <vector>

// NN Utilities
namespace NN {
    // RNGs
    extern std::mt19937 gen;
    extern std::normal_distribution<double> randn;
    extern std::uniform_real_distribution<double> rand;

    // Random initializers
    Eigen::VectorXd getBias(int);
    Eigen::MatrixXd getWeight(int, int);
}; // namespace NN

// Activation functions
enum ActFunc {
    SIGMOID,
    RELU
};

/*
 * Feedforward Neural Network
 */
class NeuralNet {
private:
    std::vector<int> sizes;        // layer sizes
    int layers;                    // number of layers
    std::vector<ActFunc> actFuncs; // activation functions for each layer

    void weightInitializer();                                    // random initializer
    Eigen::VectorXd activate(const Eigen::VectorXd &, int = -1); // apply activation functions

public:
    std::vector<Eigen::VectorXd> biases;  // NN biases
    std::vector<Eigen::MatrixXd> weights; // NN weights

    NeuralNet();
    NeuralNet(const std::vector<int> &);
    NeuralNet(const std::vector<int> &, const std::vector<Eigen::MatrixXd> &w, const std::vector<Eigen::VectorXd> &b);
    Eigen::VectorXd feedforward(Eigen::VectorXd); // get feedforward outputs
    void saveToFile(int fileOffset = 0);          // save network to file
    void loadFromFile(int fileOffset = 0);        // load network from file
};

#endif