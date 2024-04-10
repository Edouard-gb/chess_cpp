#ifndef BISHOP_H
#define BISHOP_H
#include <vector>
#include <SFML/Graphics.hpp>
#include "square.h"

class Bishop : public Piece
{
    public:
        Bishop();
        Bishop(int _color, int _x, int _y);
        Bishop(const Bishop& bishop);
        Piece* Clone();
        std::vector<Square> get_possible_squares(const Square grid[8][8]) const;
        std::vector<Square> get_attacked_squares(const Square grid[8][8]) const;
};

#endif