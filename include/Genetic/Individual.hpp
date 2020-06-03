#ifndef INDIVIDUAL_HPP
#define INDIVIDUAL_HPP

#include "Game/Game.hpp"

/*
 * Individual used as chromosome in GA
 */
class Individual : public Game {
public:
    double fitness; // fitness achieved by snake
    int score;      // score achieved by snake

    Individual();
    Individual(const std::vector<Eigen::MatrixXd> &w, const std::vector<Eigen::VectorXd> &b);
    Individual(const Individual &);
    bool operator>(const Individual &) const;
    void start();
};

#endif