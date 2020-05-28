#ifndef MAP_HPP
#define MAP_HPP

#include "Config.hpp"
#include <SFML/Graphics.hpp>
#include <unordered_map>

// VertexArray for fast rendering of whole grid
class Map : public sf::Drawable {
    static std::unordered_map<int, sf::Color> colMap;

public:
    sf::VertexArray tiles;

    Map();
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    void changeColor(int x, int y, int color);
    void updateMap(const std::vector<std::vector<int>> &grid);
};

#endif