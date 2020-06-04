#include "NN/NeuralNet.hpp"
#include "Config.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>

// Defining namespace members
std::mt19937 NN::gen = std::mt19937(std::random_device()());
std::uniform_real_distribution<double> NN::rand = std::uniform_real_distribution<double>(0.0, 1.0);
std::normal_distribution<double> NN::randn = std::normal_distribution<double>(0.0, 1.0);

// Returns biases for a layer of size m
Eigen::VectorXd NN::getBias(int m) {
    return Eigen::VectorXd(m).unaryExpr([&](__attribute__((unused)) double dummy) { return NN::randn(NN::gen); });
}

// Returns weights for layer n to m
Eigen::MatrixXd NN::getWeight(int m, int n) {
    // double div = std::sqrt(n);
    // return Eigen::MatrixXd(m, n).unaryExpr([&](__attribute__((unused)) double dummy) { return NN::randn(NN::gen) / div; });
    return Eigen::MatrixXd(m, n).unaryExpr([&](__attribute__((unused)) double dummy) { return NN::randn(NN::gen); });
}

NeuralNet::NeuralNet() {}

NeuralNet::NeuralNet(const std::vector<int> &sizes) :
    sizes(sizes),
    layers(sizes.size()) {
    actFuncs.resize(layers - 1, ActFunc::RELU);
    actFuncs[layers - 2] = ActFunc::SIGMOID;
    weightInitializer();
}

NeuralNet::NeuralNet(const std::vector<int> &sizes, const std::vector<Eigen::MatrixXd> &w, const std::vector<Eigen::VectorXd> &b) :
    sizes(sizes),
    layers(sizes.size()) {
    actFuncs.resize(layers - 1, ActFunc::RELU);
    actFuncs[layers - 2] = ActFunc::SIGMOID;
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
    switch (actFuncs[l]) {
    case ActFunc::RELU:
        return (Eigen::VectorXd)(inputs.array() < 0.0).select(0.0, inputs);
        break;
    case ActFunc::SIGMOID:
        return 1.0 / (1.0 + (inputs * -1).array().exp());
        break;
    default:
        return inputs;
    }
}

// Feedforward the inputs to get outputs
Eigen::VectorXd NeuralNet::feedforward(Eigen::VectorXd inputs) {
    for (int i = 0; i < layers - 1; i++) {
        inputs = activate((weights[i] * inputs) + biases[i], i);
    }
    return inputs;
}

void NeuralNet::saveToFile(int fileOffset) {
    std::ofstream ofs("saves/NN" + std::to_string(fileOffset) + ".net");
    for (int i = 0; i < layers - 1; i++) {
        ofs << std::setprecision(std::numeric_limits<double>::max_digits10) << weights[i] << std::endl;
        ofs << std::setprecision(std::numeric_limits<double>::max_digits10) << biases[i] << std::endl;
    }
    ofs.close();
}

void NeuralNet::loadFromFile(int fileOffset) {
    std::ifstream ifs("saves/NN" + std::to_string(fileOffset) + ".net");
    for (int l = 0; l < layers - 1; l++) {
        // First load weight matrix
        int r = weights[l].rows();
        int c = weights[l].cols();
        std::vector<double> w(c);
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                ifs >> w[j];
            }
            weights[l].row(i) = Eigen::VectorXd::Map(w.data(), w.size());
        }

        // Then load biases
        r = biases[l].rows();
        std::vector<double> b(r);
        for (int i = 0; i < r; i++) {
            ifs >> b[i];
        }
        biases[l] = Eigen::VectorXd::Map(b.data(), b.size());
    }
    ifs.close();
}
