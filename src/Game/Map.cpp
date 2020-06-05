#include "Game/Map.hpp"
#include "Config.hpp"

std::unordered_map<int, sf::Color> Map::colMap =
    std::unordered_map<int, sf::Color>({{0, sf::Color::Black}, // empty tile
                                        {1, sf::Color(255, 20, 147)},
                                        {11, sf::Color(155, 20, 47, 100)},
                                        {2, sf::Color(0, 255, 255, 255)},
                                        {12, sf::Color(0, 100, 100, 50)},
                                        {3, sf::Color(0, 255, 0)},
                                        {13, sf::Color(0, 155, 0, 70)},
                                        {4, sf::Color(240, 220, 110)},
                                        {14, sf::Color(20, 25, 0)},
                                        {-1, sf::Color::Red}}); // food tile

Map::Map() {
    int width = Config::COLS;
    int height = Config::ROWS;
    int tilesize = Config::WIDTH / Config::COLS;

    tiles.setPrimitiveType(sf::Quads);
    tiles.resize(width * height * 4);

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int tilenumber = i + j * width;
            sf::Vertex *quad = &tiles[tilenumber * 4];

            quad[0].position = sf::Vector2f(i * tilesize, j * tilesize);
            quad[1].position = sf::Vector2f((i + 1) * tilesize, j * tilesize);
            quad[2].position = sf::Vector2f((i + 1) * tilesize, (j + 1) * tilesize);
            quad[3].position = sf::Vector2f(i * tilesize, (j + 1) * tilesize);

            quad[0].color = sf::Color::Black;
            quad[1].color = sf::Color::Black;
            quad[2].color = sf::Color::Black;
            quad[3].color = sf::Color::Black;
        }
    }
}

// Update Map at (x, y)
void Map::changeColor(int x, int y, int color) {
    int tilenumber = x + y * Config::COLS;
    sf::Vertex *quad = &tiles[tilenumber * 4];
    quad[0].color = colMap[color];
    quad[1].color = colMap[color];
    quad[2].color = colMap[color];
    quad[3].color = colMap[color];
}

// Update whole 2D Map
void Map::updateMap(const std::vector<std::vector<int>> &grid) {
    int width = Config::COLS;
    int height = Config::ROWS;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int tilenumber = i + j * width;
            sf::Vertex *quad = &tiles[tilenumber * 4];
            quad[0].color = colMap[grid[j][i]];
            quad[1].color = colMap[grid[j][i]];
            quad[2].color = colMap[grid[j][i]];
            quad[3].color = colMap[grid[j][i]];
        }
    }
}

// Overriden abstract function definition
void Map::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(tiles, states);
}
