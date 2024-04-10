#include <iostream>
#include "piece.h"

Piece::Piece() {}

Piece::Piece(const Piece& other_piece){
    x = other_piece.x;
    y = other_piece.y;
    color = other_piece.color;
    value = other_piece.value;
    is_alive = other_piece.is_alive;
}

Piece::~Piece(){}

Piece::Piece(int _color, int _x, int _y)
{
    is_alive = true;
    x = _x;
    y = _y;
    color = _color;
}

void Piece::set_position(int _x, int _y){
    x = _x;
    y = _y;
    piece.setPosition(x * 100.f + 50.f, y * 100.f + 50.f);
}

void Piece::dies(){
    x = -1;
    y = -1;
    is_alive = false;
    piece.setPosition(-1 * 100.f + 50.f, -1 * 100.f + 50.f);
}

bool Piece::operator == (const Piece& piece) {
    if (x == piece.x and y == piece.y and color == piece.color and value == piece.value){
        return true;
    }
    return false;
}
