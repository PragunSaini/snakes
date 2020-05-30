#include "NN/NeuralNet.hpp"
#include <iostream>

std::mt19937 NN::gen = std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());
std::uniform_real_distribution<double> NN::rand = std::uniform_real_distribution<double>(0.0, 1.0);
std::normal_distribution<double> NN::randn = std::normal_distribution<double>(0.0, 1.0);

double NN::relu(double z) {
    return std::max(0.0, z);
}

double NN::sigmoid(double z) {
    return 1.0 / (1.0 + std::exp(-z));
}

// Returns biases for a layer of size m
Eigen::VectorXd NN::getBias(int m) {
    return Eigen::VectorXd(m).unaryExpr([&](__attribute__((unused)) double dummy) { return NN::randn(NN::gen); });
}

// Returns weights for layer n to m
Eigen::MatrixXd NN::getWeight(int m, int n) {
    double div = std::sqrt(n);
    return Eigen::MatrixXd(m, n).unaryExpr([&](__attribute__((unused)) double dummy) { return NN::randn(NN::gen) / div; });
}

NeuralNet::NeuralNet() {}

NeuralNet::NeuralNet(const std::vector<int> &sizes) :
    sizes(sizes),
    layers(sizes.size()) {
    actFuncs.resize(layers - 1, NN::sigmoid);
    actFuncs[layers - 2] = NN::relu;
    weightInitializer();
}

NeuralNet::NeuralNet(const std::vector<int> &sizes, const std::vector<Eigen::MatrixXd> &w, const std::vector<Eigen::VectorXd> &b) :
    sizes(sizes),
    layers(sizes.size()) {
    actFuncs.resize(layers - 1, NN::sigmoid);
    actFuncs[layers - 2] = NN::relu;
    weights = w;
    biases = b;
}

void NeuralNet::weightInitializer() {
    biases.resize(layers - 1);
    for (int i = 1; i < layers; i++) {
        biases[i - 1] = NN::getBias(sizes[i]);
    }

    weights.resize(layers - 1);
    for (int i = 1; i < layers; i++) {
        weights[i - 1] = NN::getWeight(sizes[i], sizes[i - 1]);
    }
}

Eigen::VectorXd NeuralNet::activate(const Eigen::VectorXd &inputs, int l) {
    if (l < layers - 2) {
        return 1.0 / (1.0 + (inputs * -1).array().exp());
    }
    // else if (l == size)
    else {
        // return inputs.unaryExpr(actFuncs[l]);
        return inputs.unaryExpr([&](double z){return std::max(0.0, z);});
    }
}

// Feedforward the inputs to get outputs
// WAT THE FAK IS HAPPENING
Eigen::VectorXd NeuralNet::feedforward(Eigen::VectorXd inputs) {
    for (int i = 0; i < layers - 1; i++) {
        inputs = activate((weights[i] * inputs) + biases[i], i);
    }
    return inputs;
}