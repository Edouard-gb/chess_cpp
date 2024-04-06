#ifndef KING_H
#define KING_H
#include <vector>
#include <SFML/Graphics.hpp>
#include "square.h"

class King : public Piece
{
    public:
        King();
        King(int _color);
        King(int _color, int _x, int _y);
        std::vector<Square> get_possible_squares(Square grid[8][8]) const;
};

#endif