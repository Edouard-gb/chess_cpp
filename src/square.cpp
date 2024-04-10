#include "square.h"

Square::Square()
{
    occupant = nullptr;
}

Square::Square(int _x, int _y)
{
    x = _x;
    y = _y;
    occupant = nullptr;
    square.setPosition(sf::Vector2f(x * 100.0f, y * 100.0f));
    square.setSize(sf::Vector2f(100.f, 100.f));
    square.setFillColor((y + x) % 2 ? sf::Color(0xae722bff) : sf::Color(0xf3bc7aff));
}

Square::Square(int _x, int _y, Piece* piece)
{
    x = _x;
    y = _y;
    occupant = piece;
    square.setPosition(sf::Vector2f(x * 100.0f, y * 100.0f));
    square.setSize(sf::Vector2f(100.f, 100.f));
    square.setFillColor((y + x) % 2 ? sf::Color(0xae722bff) : sf::Color(0xf3bc7aff));
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