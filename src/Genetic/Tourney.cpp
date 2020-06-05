#include "Genetic/Tourney.hpp"
#include "Config.hpp"

Tourney::Tourney() :
    Individual(),
    players(Config::PLAYERCNT) {
    for (Player &player : players) {
        player.initNeuralNet();
    }
}

Tourney::Tourney(const Tourney &other) :
    Individual(other),
    players(Config::PLAYERCNT) {
    for (int i = 0; i < Config::PLAYERCNT; i++) {
        players[i].net = other.players[0].net;
    }
}

Tourney &Tourney::operator=(const Tourney &other) {
    Individual::operator=(other);
    players = std::vector<Player>(Config::PLAYERCNT);
    for (int i = 0; i < Config::PLAYERCNT; i++) {
        players[i].net = other.players[0].net;
    }
    return *this;
}

void Tourney::initObjects() {
    for (Player &player : players) {
        player.initBody(this);
    }
    foodManager.generateFood(this, Config::FOODCNT_T);
}

void Tourney::start() {
    initObjects();
    while (true) {
        bool updated = false;
        for (Player &player : players) {
            if (player.alive) {
                updated = true;
                player.look(grid);
                player.determineMove();
            }
        }
        for (Player &player : players) {
            if (player.alive)
                player.move(grid);
        }
        if (!updated)
            break;
    }
    for (Player &player : players) {
        fitness += player.fitness;
        score = score > player.score ? score : player.score;
    }
    fitness = fitness + std::pow(score, 2);
}

void Tourney::saveToFile() {
    int i = 0;
    for (Player &player : players) {
        player.net.saveToFile(i++);
    }
}
