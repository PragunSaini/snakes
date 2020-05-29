#ifndef INDIVIDUAL_HPP
#define INDIVIDUAL_HPP

#include "Game.hpp"
#include <iostream>
#include <vector>

class Individual : public Game {
private:
    // void init(const std::vector<Eigen::MatrixXd> &w = {}, const std::vector<Eigen::VectorXd> &b = {});

public:
    double fitness;

    Individual();
    Individual(const std::vector<Eigen::MatrixXd> &w, const std::vector<Eigen::VectorXd> &b);
    Individual(const Individual &);
    Individual &operator=(const Individual &);
    bool operator>(const Individual &) const;
    void start();
};

#endif