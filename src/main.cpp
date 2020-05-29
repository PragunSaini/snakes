// #include "Genetic/Genetic.hpp"
#include "Config.hpp"
#include "Genetic/Genetic.hpp"
#include "NN/NeuralNet.hpp"
#include "Render.hpp"
#include <iostream>

int main() {
    GeneticAlgo gen;
    gen.start();
    // Render rend;
    // rend.start();
    // NeuralNet net({4, 10, 20, 10, 2});
    // Eigen::Vector4d x = Eigen::Vector4d(1, 2, 3, 4);
    // Eigen::VectorXd ans = net.feedforward(x);
    // std::cout << ans << std::endl;

    // NeuralNet nana({4, 10, 20, 10, 2}, net.weights, net.biases);
    // std::cout << nana.feedforward(x) << std::endl;
}
