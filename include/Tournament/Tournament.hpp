#ifndef TOURNAMENT_HPP
#define TOURNAMENT_HPP

#include "Game/Game.hpp"
#include "Game/Map.hpp"
#include "Tournament/Player.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

/*
 * To hold tournament between multiple snakes
 */
class Tournament : public Game {
private:
    int playerCount;             // number of players
    std::vector<Player> players; // players in the tournament
    sf::RenderWindow window;     // window to render
    Map map;                     // VertexArray for fast grid rendering
    int tilesize;                // Tilesize of a single grid block
    sf::Time snakeSpeed;         // Snake speed
    sf::Clock snakeUpdate;       // To track snake updates
    sf::Font font;               // font for window text
    sf::Text bestScore;          // used to display best score
    sf::Clock fps;               // to display FPS

    void initializeWindow(); // used to setup window
    void draw();             // Draw the screen

public:
    Tournament(const std::vector<Eigen::MatrixXd> &w = {}, const std::vector<Eigen::VectorXd> &b = {});
    void initObjects();
    void start();
};

#endif