#ifndef BISHOP_H
#define BISHOP_H
#include <vector>
#include <SFML/Graphics.hpp>
#include "square.h"

class Bishop : public Piece
{
    public:
        Bishop();
        Bishop(int _color);
        Bishop(int _color, int _x, int _y);
        std::vector<Square> get_possible_squares(Square grid[8][8]) const;
};

#endif