#include <iostream>
#include "piece.h"

Piece::Piece() {}

Piece::Piece(int _color)
{
    is_alive = 1;
    color = _color;
}

Piece::Piece(int _color, int _x, int _y)
{
    is_alive = 1;
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
    is_alive = 0;
    piece.setPosition(-1 * 100.f + 50.f, -1 * 100.f + 50.f);
}