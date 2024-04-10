#ifndef KNIGHT_H
#define KNIGHT_H
#include <vector>
#include <SFML/Graphics.hpp>
#include "square.h"

class Knight : public Piece
{
    public:
        Knight();
        Knight(int _color, int _x, int _y);
        Knight(const Knight& knight);
        Piece* Clone();
        std::vector<Square> get_possible_squares(const Square grid[8][8]) const;
        std::vector<Square> get_attacked_squares(const Square grid[8][8]) const;
};

#endif