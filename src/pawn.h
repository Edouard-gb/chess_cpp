#ifndef PAWN_H
#define PAWN_H
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "square.h"


class Pawn : public Piece
{
    public:
        Pawn();
        Pawn(int _color);
        Pawn(int _color, int _x, int _y);
        std::vector<Square> get_possible_squares(Square grid[8][8]) const;
};

#endif