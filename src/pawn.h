#ifndef PAWN_H
#define PAWN_H
#include <vector>
#include <SFML/Graphics.hpp>
#include "square.h"


class Pawn : public Piece
{
    public:
        Pawn();
        Pawn(int _color, int _x, int _y);
        Pawn(const Pawn& pawn);
        Piece* Clone();
        std::vector<Square> get_possible_squares(const Square grid[8][8]) const;
        std::vector<Square> get_attacked_squares(const Square grid[8][8]) const;
};

#endif