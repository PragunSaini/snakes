#ifndef NEURAL_NET_HPP
#define NEURAL_NET_HPP

#include <chrono>
#include <cmath>
#include <random>
#include <vector>

namespace ActivationFuncs {
    double sigmoid(double);
    double relu(double);

} // namespace ActivationFuncs

class RandomHelper {
public:
    std::mt19937 gen;
    std::normal_distribution<double> randn;

    RandomHelper();
    std::vector<double> getBias(int);
    std::vector<std::vector<double>> getWeight(int, int);
};

class NeuralNet {
public:
    typedef std::vector<double> VecBiases;
    typedef std::vector<std::vector<double>> VecWeights;

private:
    std::vector<int> sizes;
    int layers;
    std::vector<double (*)(double)> actFuncs;
    RandomHelper random;

    void weightInitializer();
    std::vector<double> dot(const VecWeights &, const std::vector<double> &);
    std::vector<double> add(const std::vector<double> &, const VecBiases &);
    std::vector<double> activate(const std::vector<double> &, int = -1);

public:
    std::vector<VecBiases> biases;
    std::vector<VecWeights> weights;
    NeuralNet();
    NeuralNet(const std::vector<int> &);
    NeuralNet(const std::vector<int> &, const std::vector<VecWeights> &w, const std::vector<VecBiases> &b);
    std::vector<double> feedforward(std::vector<double>);
};

#endif