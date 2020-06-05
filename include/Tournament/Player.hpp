#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Game/Snake.hpp"

/*
 * A tounament player snake
 */
class Player : public Snake {
private:
    bool isValidPosition(int, int, Direction, Game *);
    void calculateFitness();

public:
    void initBody(Game *);
};

#endif