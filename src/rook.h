#ifndef ROOK_H
#define ROOK_H
#include <vector>
#include <SFML/Graphics.hpp>
#include "square.h"

class Rook : public Piece
{
    public:
        Rook();
        Rook(int _color);
        Rook(int _color, int _x, int _y);
        std::vector<Square> get_possible_squares(Square grid[8][8]) const;
};

#endif