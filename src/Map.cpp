#include "Map.hpp"
#include "Game.hpp"
#include <iostream>

Map::Map(Game *game) :
    game(game) {

    colMap = std::unordered_map<int, sf::Color>({{0, sf::Color::Black},
                                                 {1, sf::Color::Magenta},
                                                 {2, sf::Color::Cyan},
                                                 {3, sf::Color::Green},
                                                 {-1, sf::Color::Red}});

    int width = Config::COLS;
    int height = Config::ROWS;
    int tilesize = game->tilesize;

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

void Map::changeColor(int x, int y) {
    int tilenumber = x + y * Config::COLS;
    sf::Vertex *quad = &tiles[tilenumber * 4];
    quad[0].color = colMap[game->grid[y][x]];
    quad[1].color = colMap[game->grid[y][x]];
    quad[2].color = colMap[game->grid[y][x]];
    quad[3].color = colMap[game->grid[y][x]];
}

void Map::updateMap() {
    int width = Config::COLS;
    int height = Config::ROWS;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int tilenumber = i + j * width;
            sf::Vertex *quad = &tiles[tilenumber * 4];
            quad[0].color = colMap[game->grid[j][i]];
            quad[1].color = colMap[game->grid[j][i]];
            quad[2].color = colMap[game->grid[j][i]];
            quad[3].color = colMap[game->grid[j][i]];
        }
    }
}

void Map::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(tiles, states);
}
