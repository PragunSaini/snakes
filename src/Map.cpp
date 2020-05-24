#include "Map.hpp"
#include "Game.hpp"
#include <iostream>

Map::Map(Game *game) :
    game(game) {
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
    int col = game->grid[y][x];
    sf::Vertex *quad = &tiles[tilenumber * 4];
    quad[0].color = col == 0 ? sf::Color::Black : sf::Color::Yellow;
    quad[1].color = col == 0 ? sf::Color::Black : sf::Color::Yellow;
    quad[2].color = col == 0 ? sf::Color::Black : sf::Color::Yellow;
    quad[3].color = col == 0 ? sf::Color::Black : sf::Color::Yellow;
}

void Map::updateMap() {
    int width = Config::COLS;
    int height = Config::ROWS;
    int tilesize = game->tilesize;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int tilenumber = i + j * width;
            sf::Vertex *quad = &tiles[tilenumber * 4];
            int col = game->grid[j][i];
            quad[0].color = col == 0 ? sf::Color::Black : sf::Color::Yellow;
            quad[1].color = col == 0 ? sf::Color::Black : sf::Color::Yellow;
            quad[2].color = col == 0 ? sf::Color::Black : sf::Color::Yellow;
            quad[3].color = col == 0 ? sf::Color::Black : sf::Color::Yellow;
        }
    }
}

void Map::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(tiles, states);
}
