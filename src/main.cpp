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
                    if (e.mouseButton.x >= 0 && e.mouseButton.x <= 800 && e.mouseButton.y >= 0 && e.mouseButton.y <= 800){
                        int y = e.mouseButton.y / 100, x = e.mouseButton.x / 100;
                        if (!chess.is_piece_selected() && !chess.is_game_over){
                            chess.select_piece(x, y);
                        }
                        else {
                            chess.make_move(x, y);
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