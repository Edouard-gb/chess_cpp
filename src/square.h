#ifndef SQUARE_H
#define SQUARE_H
#include <SFML/Graphics.hpp>
#include "piece.h"


class Square
{
    public:
        Square();
        Square(int _x, int _y);
        Square(int _x, int _y, Piece* piece);

        void set_occupant(Piece* piece);
        bool is_occupied() const;
        bool operator == (const Square &square);

        int x, y;
        Piece* occupant;
        sf::RectangleShape square;
        sf::CircleShape circle_move_indicator;
};

#endif