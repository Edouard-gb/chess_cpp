#ifndef KING_H
#define KING_H
#include <vector>
#include <SFML/Graphics.hpp>
#include "square.h"

class King : public Piece
{
    public:
        King();
        King(int _color, int _x, int _y);
        King(const King& king);
        Piece* Clone();
        void set_position(int _x, int _y) override;
        std::vector<Square> get_possible_squares(const Square grid[8][8]) const;
        std::vector<Square> get_attacked_squares(const Square grid[8][8]) const;
        bool has_moved_once = false;
};

#endif