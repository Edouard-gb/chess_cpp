#ifndef QUEEN_H
#define QUEEN_H
#include <vector>
#include <SFML/Graphics.hpp>
#include "square.h"

class Queen : public Piece
{
    public:
        Queen();
        Queen(int _color, int _x, int _y);
        Queen(const Queen& queen);
        Piece* Clone();
        std::vector<Square> get_possible_squares(const Square grid[8][8]) const;
        std::vector<Square> get_attacked_squares(const Square grid[8][8]) const;
};

#endif