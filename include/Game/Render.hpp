#ifndef RENDER_HPP
#define RENDER_HPP

#include "Game/Game.hpp"
#include "Game/Map.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

/**
 * Renders a game
 */
class Render : public Game {
private:
    bool vision;             // whether to use snake's vision or not
    sf::RenderWindow window; // window to render
    Map map;                 // VertexArray for fast grid rendering
    int tilesize;            // Tilesize of a single grid block
    sf::Time snakeSpeed;     // Snake speed
    sf::Clock snakeUpdate;   // To track snake updates
    sf::Font font;           // font for window text
    sf::Text score;          // used to display score
    sf::Clock fps;           // to display FPS

    void initializeWindow();      // used to set framerate
    void draw();                  // Draw the screen
    void handleEvents(sf::Event); // Handle key events

public:
    Render(bool = false);
    Render(const std::vector<Eigen::MatrixXd> &w, const std::vector<Eigen::VectorXd> &b);
    Render(const Render &);
    void start();
};

#endif