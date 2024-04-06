#ifndef PIECE_H
#define PIECE_H
#include <vector>
#include <SFML/Graphics.hpp>

class Square;

class Piece
{
    public:
        Piece();
        Piece(int _color);
        Piece(int _color, int _x, int _y);
        // grid coordinates
        int x, y;
        // 1 if white, 0 if black piece.
        int color;
        // 5: king, 4: Queen, 3: Rook, 2: Knight, 1: Bishop, 0: Pawn.
        int value;
        // 1 for alive, 0 for dead.
        int is_alive;
        sf::Sprite piece;
        sf::Texture texture;

        void dies();
        void set_position(int _x, int _y);
        virtual std::vector<Square> get_possible_squares(Square grid[8][8]) const = 0;
};

#endif