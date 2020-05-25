#ifndef NEURAL_NET_HPP
#define NEURAL_NET_HPP

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
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
    typedef std::vector<double> VecBiases;
    typedef std::vector<std::vector<double>> VecWeights;

    std::vector<int> sizes;
    int layers;
    std::vector<double (*)(double)> actFuncs;
    std::vector<VecBiases> biases;
    std::vector<VecWeights> weights;
    RandomHelper random;

    void weightInitializer();
    std::vector<double> dot(VecWeights, std::vector<double>);
    std::vector<double> add(std::vector<double>, VecBiases);
    std::vector<double> activate(std::vector<double> inputs, int = -1);

public:
    NeuralNet(std::vector<int>);
    std::vector<double> feedforward(std::vector<double>);
};

#endif