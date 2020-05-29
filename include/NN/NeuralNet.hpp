#ifndef NEURAL_NET_HPP
#define NEURAL_NET_HPP

#include <Eigen/Dense>
#include <chrono>
#include <cmath>
#include <random>
#include <vector>

// Activation Functions
namespace NN {
    extern std::mt19937 gen;
    extern std::normal_distribution<double> randn;
    extern std::uniform_real_distribution<double> rand;

    double sigmoid(double);
    double relu(double);

    Eigen::VectorXd getBias(int);
    Eigen::MatrixXd getWeight(int, int);
}; // namespace NN

// Feedforward Neural Network
class NeuralNet {

private:
    std::vector<int> sizes;
    int layers;
    std::vector<double (*)(double)> actFuncs;

    void weightInitializer();
    std::vector<double> dot(const Eigen::MatrixXd &, const Eigen::VectorXd &);
    std::vector<double> add(const Eigen::VectorXd &, const Eigen::VectorXd &);
    Eigen::VectorXd activate(const Eigen::VectorXd &, int = -1);

public:
    std::vector<Eigen::VectorXd> biases;
    std::vector<Eigen::MatrixXd> weights;
    NeuralNet();
    NeuralNet(const std::vector<int> &);
    NeuralNet(const std::vector<int> &, const std::vector<Eigen::MatrixXd> &w, const std::vector<Eigen::VectorXd> &b);
    Eigen::VectorXd feedforward(Eigen::VectorXd);
};

#endif