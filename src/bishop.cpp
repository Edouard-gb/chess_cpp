#include "bishop.h"
#include <iostream>

Bishop::Bishop() {
    value = 1;
}

Bishop::Bishop(int _color, int _x, int _y): Piece(_color, _x, _y)
{
    value = 1;
    texture.loadFromFile((_color == 0) ? "../src/Textures/b_bishop.png" : "../src/Textures/w_bishop.png");
    piece.setTexture(texture);
    piece.setPosition(_x * 100.f + 50.f, _y * 100.f + 50.f);
    piece.setOrigin(sf::Vector2f(piece.getTexture()->getSize().x / 2, piece.getTexture()->getSize().y / 2));
    piece.setScale(sf::Vector2f(0.5f, 0.5f));
}

Bishop::Bishop(const Bishop& bishop): Piece(bishop){}

Piece* Bishop::Clone(){
    return new Bishop(*this);
}

std::vector<Square> Bishop::get_possible_squares(const Square grid[8][8]) const
{
    std::vector<Square> result;
    int directions[4][2] = { // matrix notation: {x_dir, y_ydir}
        { 1, -1}, //NE
        {-1, -1}, //NW
        { 1,  1}, //SE
        {-1,  1}, //SW
    };
    int x_dir;
    int y_dir;

    for (int dir_index = 0; dir_index < 4; dir_index++){
        x_dir = directions[dir_index][0];
        y_dir = directions[dir_index][1];
        while (0 <= x + x_dir and x + x_dir <= 7 and 0 <= y + y_dir and y + y_dir <= 7) {
            if (grid[y + y_dir][x + x_dir].is_occupied()) {
                if (grid[y + y_dir][x + x_dir].occupant->color != color){
                    result.push_back(Square(x + x_dir, y + y_dir));
                }
                break;
            }
            result.push_back(Square(x + x_dir, y + y_dir));
            x_dir += directions[dir_index][0];
            y_dir += directions[dir_index][1];
        }
    }
    return result;
}

std::vector<Square> Bishop::get_attacked_squares(const Square grid[8][8]) const {
    return get_possible_squares(grid);
}