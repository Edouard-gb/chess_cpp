#ifndef ROOK_H
#define ROOK_H
#include <vector>
#include <SFML/Graphics.hpp>
#include "square.h"

class Rook : public Piece
{
    public:
        Rook();
        Rook(int _color, int _x, int _y, bool _is_king_side_rook);
        Rook(const Rook& rook);
        Piece* Clone();
        void set_position(int _x, int _y) override;
        std::vector<Square> get_possible_squares(const Square grid[8][8]) const;
        std::vector<Square> get_attacked_squares(const Square grid[8][8]) const;
        bool is_king_side_rook;
        bool has_moved_once = false;
};

#endif