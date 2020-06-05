#ifndef TOURNEY_HPP
#define TOURNEY_HPP

#include "Genetic/Individual.hpp"
#include "Tournament/Player.hpp"

/*
 * Chromosome representing a tournament (WIP)
 */
class Tourney : public Individual {
public:
    std::vector<Player> players;
    void initObjects();

    Tourney();
    Tourney(const Tourney &);
    Tourney &operator=(const Tourney &);
    void start();
    void saveToFile();
};

#endif