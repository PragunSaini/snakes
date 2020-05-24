#ifndef MAP_HPP
#define MAP_HPP

#include "Config.hpp"
#include <SFML/Graphics.hpp>

// forward declaration
class Game;

class Map : public sf::Drawable {
public:
    Game *game;
    sf::VertexArray tiles;

    Map(Game *);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    void changeColor(int x, int y);
};

#endif