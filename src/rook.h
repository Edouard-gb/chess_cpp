#ifndef ROOK_H
#define ROOK_H
#include <vector>
#include <SFML/Graphics.hpp>
#include "square.h"

class Rook : public Piece
{
    public:
        Rook();
        Rook(int _color, int _x, int _y);
        Rook(const Rook& rook);
        Piece* Clone();
        std::vector<Square> get_possible_squares(const Square grid[8][8]) const;
        std::vector<Square> get_attacked_squares(const Square grid[8][8]) const;
};

#endif