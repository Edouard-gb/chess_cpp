#include "square.h"

Square::Square()
{
    occupant = nullptr;
}

Square::Square(int _x, int _y, bool with_graphics)
{
    x = _x;
    y = _y;
    occupant = nullptr;
    if (with_graphics) {
        square.setPosition(sf::Vector2f(x * 100.0f, y * 100.0f));
        square.setSize(sf::Vector2f(100.f, 100.f));
        square.setFillColor((y + x) % 2 ? sf::Color(0xae722bff) : sf::Color(0xf3bc7aff));

        circle_move_indicator.setRadius(12);
        circle_move_indicator.setFillColor(sf::Color(105, 105, 105, 80));
        circle_move_indicator.setOrigin(12, 12);
        circle_move_indicator.setPosition(sf::Vector2f(x * 100.0f + 50, y * 100.0f + 50));
    }
}

Square::Square(int _x, int _y, Piece* piece)
{
    x = _x;
    y = _y;
    occupant = piece;

    square.setPosition(sf::Vector2f(x * 100.0f, y * 100.0f));
    square.setSize(sf::Vector2f(100.f, 100.f));
    square.setFillColor((y + x) % 2 ? sf::Color(0xae722bff) : sf::Color(0xf3bc7aff));

    circle_move_indicator.setRadius(12);
    circle_move_indicator.setFillColor(sf::Color(105, 105, 105, 80));
    circle_move_indicator.setOrigin(12, 12);
    circle_move_indicator.setPosition(sf::Vector2f(x * 100.0f + 50, y * 100.0f + 50));
}

void Square::set_occupant(Piece* piece)
{
    occupant = piece;
}

bool Square::is_occupied() const 
{
    return occupant != nullptr;
}

bool Square::operator == (const Square& square) {
    if (x == square.x and y == square.y){
        return true;
    }
    return false;
}