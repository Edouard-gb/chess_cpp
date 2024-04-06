#ifndef QUEEN_H
#define QUEEN_H
#include <vector>
#include <SFML/Graphics.hpp>
#include "square.h"

class Queen : public Piece
{
    public:
        Queen();
        Queen(int _color);
        Queen(int _color, int _x, int _y);
        std::vector<Square> get_possible_squares(Square grid[8][8]) const;
};

#endif