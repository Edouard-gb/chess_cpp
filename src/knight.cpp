#include "knight.h"
#include <iostream>

Knight::Knight() {
    value = 2;
}

Knight::Knight(int _color, int _x, int _y): Piece(_color, _x, _y)
{
    value = 2;
    texture.loadFromFile((_color == 0) ? "../src/Textures/b_knight.png" : "../src/Textures/w_knight.png");
    piece.setTexture(texture);
    piece.setPosition(_x * 100.f + 50.f, _y * 100.f + 50.f);
    piece.setOrigin(sf::Vector2f(piece.getTexture()->getSize().x / 2, piece.getTexture()->getSize().y / 2));
    piece.setScale(sf::Vector2f(0.5f, 0.5f));
}

Knight::Knight(const Knight& knight): Piece(knight){}

Piece* Knight::Clone(){
    return new Knight(*this);
}

std::vector<Square> Knight::get_possible_squares(const Square grid[8][8]) const
{
    std::vector<Square> result;
    int directions[8][2] = { // matrix notation: {x_dir, y_ydir}
        { 1,  2},
        { 2,  1},
        {-1, -2},
        {-2, -1},
        { 1, -2},
        { 2, -1},
        {-1,  2},
        {-2,  1},
    };
    int x_dir;
    int y_dir;

    for (int dir_index = 0; dir_index < 8; dir_index++){
        x_dir = directions[dir_index][0];
        y_dir = directions[dir_index][1];
        if (0 <= x + x_dir and x + x_dir <= 7 and 0 <= y + y_dir and y + y_dir <= 7) {
            if (grid[y + y_dir][x + x_dir].is_occupied()) {
                if (grid[y + y_dir][x + x_dir].occupant->color != color){
                    result.push_back(Square(x + x_dir, y + y_dir));
                }
                continue;
            }
            result.push_back(Square(x + x_dir, y + y_dir));
        }
    }
    return result;
}

std::vector<Square> Knight::get_attacked_squares(const Square grid[8][8]) const {
    return this->get_possible_squares(grid);
}
