#include "pawn.h"
#include <iostream>

Pawn::Pawn() {
    value = 0;
}

Pawn::Pawn(int _color, int _x, int _y): Piece(_color, _x, _y)
{
    value = 0;
    texture.loadFromFile((_color == 0) ? "../src/Textures/b_pawn.png" : "../src/Textures/w_pawn.png");
    piece.setTexture(texture);
    piece.setOrigin(sf::Vector2f(piece.getTexture()->getSize().x / 2, piece.getTexture()->getSize().y / 2));
    piece.setScale(sf::Vector2f(0.375f, 0.375f));
    piece.setPosition(_x * 100.f + 50.f, _y * 100.f + 50.f);
}

Pawn::Pawn(const Pawn& pawn): Piece(pawn){}

Piece* Pawn::Clone(){
    return new Pawn(*this);
}

std::vector<Square> Pawn::get_possible_squares(const Square grid[8][8]) const{
    int y_dir = (color == 0) ? 1 : -1;
    std::vector<Square> result;

    if (!grid[y + y_dir][x].is_occupied()) {
        result.push_back(Square(x, y + y_dir));
    }

    if (!grid[y + y_dir * 2][x].is_occupied() && y == ((color == 0) ? 1 : 6)) {
        result.push_back(Square(x, y + y_dir * 2));
    }

    if (x > 0 && grid[y + y_dir][x - 1].is_occupied()) {
        if (grid[y + y_dir][x - 1].occupant->color != color) {
            result.push_back(Square(x - 1, y + y_dir));
        }
    }

    if (x < 7 && grid[y + y_dir][x + 1].is_occupied()) {
        if (grid[y + y_dir][x + 1].occupant->color != color) {
            result.push_back(Square(x + 1, y + y_dir));
        }
    }

    return result;
}

std::vector<Square> Pawn::get_attacked_squares(const Square grid[8][8]) const {
    int y_dir = (color == 0) ? 1 : -1;
    std::vector<Square> result;
    result.push_back(Square(x - 1, y + y_dir));
    result.push_back(Square(x + 1, y + y_dir));
    return result;
}
