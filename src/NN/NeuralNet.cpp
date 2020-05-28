#include "NN/NeuralNet.hpp"

double ActivationFuncs::relu(double z) {
    return std::max(0.0, z);
}

double ActivationFuncs::sigmoid(double z) {
    return 1.0 / (1.0 + std::exp(-z));
}

RandomHelper::RandomHelper() :
    gen(std::mt19937(std::chrono::system_clock::now().time_since_epoch().count())),
    randn(std::normal_distribution<double>(0.0, 1.0)) {}

// Returns biases for a layer of size m
std::vector<double> RandomHelper::getBias(int m) {
    std::vector<double> biases(m);
    for (int i = 0; i < m; i++) {
        biases[i] = randn(gen);
    }
    return biases;
}

// Returns weights for layer n to m
std::vector<std::vector<double>> RandomHelper::getWeight(int m, int n) {
    std::vector<std::vector<double>> weights(m, std::vector<double>(n));
    double div = std::sqrt(n);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            weights[i][j] = randn(gen) / div;
        }
    }
    return weights;
}

NeuralNet::NeuralNet() {}

NeuralNet::NeuralNet(const std::vector<int> &sizes) :
    sizes(sizes),
    layers(sizes.size()) {
    actFuncs.resize(layers - 1, ActivationFuncs::sigmoid);
    actFuncs[layers - 2] = ActivationFuncs::relu;
    weightInitializer();
}

NeuralNet::NeuralNet(const std::vector<int> &sizes, const std::vector<VecWeights> &w, const std::vector<VecBiases> &b) :
    sizes(sizes),
    layers(sizes.size()) {
    actFuncs.resize(layers - 1, ActivationFuncs::sigmoid);
    actFuncs[layers - 2] = ActivationFuncs::relu;
    weights = w;
    biases = b;
}

void NeuralNet::weightInitializer() {
    biases.resize(layers - 1);
    for (int i = 1; i < layers; i++) {
        biases[i - 1] = random.getBias(sizes[i]);
    }

    weights.resize(layers - 1);
    for (int i = 1; i < layers; i++) {
        weights[i - 1] = random.getWeight(sizes[i], sizes[i - 1]);
    }
}

std::vector<double> NeuralNet::dot(const VecWeights &w, const std::vector<double> &inputs) {
    int m = w.size();
    int n = w[0].size();
    std::vector<double> res(m);
    for (int i = 0; i < m; i++) {
        double sum = 0;
        for (int j = 0; j < n; j++) {
            sum += w[i][j] * inputs[j];
        }
        res[i] = sum;
    }
    return res;
}

std::vector<double> NeuralNet::add(const std::vector<double> &inputs, const VecBiases &b) {
    int m = b.size();
    std::vector<double> res(m);
    for (int i = 0; i < m; i++) {
        res[i] = inputs[i] + b[i];
    }
    return res;
}

std::vector<double> NeuralNet::activate(const std::vector<double> &inputs, int layer) {
    double (*actFunc)(double) = layer == -1 ? ActivationFuncs::sigmoid : actFuncs[layer];
    int m = inputs.size();
    std::vector<double> res(m);
    for (int i = 0; i < m; i++) {
        res[i] = actFunc(inputs[i]);
    }
    return res;
}

// Feedforward the inputs to get outputs
std::vector<double> NeuralNet::feedforward(std::vector<double> inputs) {
    for (int i = 0; i < layers - 1; i++) {
        inputs = activate(add(dot(weights[i], inputs), biases[i]), i);
    }
    return inputs;
}