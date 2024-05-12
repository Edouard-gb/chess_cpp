#include <iostream>
#include <SFML/Graphics.hpp>
#include "game.h"

int main(int argc, char * argv[])
{
std::cout << argv[0];
std::cin.get();
label:


    sf::RenderWindow window(sf::VideoMode(800, 800), "The Chess", sf::Style::Titlebar | sf::Style::Close);
    Game chess;
    while (window.isOpen()){
        sf::Event e;
        while (window.pollEvent(e)){
            if (e.type == sf::Event::Closed){
                window.close();
            }
            if (e.type == sf::Event::MouseButtonPressed){
                if (e.mouseButton.button == sf::Mouse::Left){
                    int y = e.mouseButton.y, x = e.mouseButton.x;
                    if (x >= 0 and x <= 800 and y >= 0 and y <= 800){
                        if (chess.awaiting_promotion_at_x != -1){
                            chess.promote(x, y);
                        } else if (!chess.is_piece_selected() and !chess.is_game_over){
                            chess.select_piece(x / 100, y / 100);
                        } else if (!chess.is_game_over){
                            chess.make_move(x / 100, y / 100);
                        }
                    }
                }
            }
        }
        window.draw(chess);
        window.display();
    }
    return 0;
}