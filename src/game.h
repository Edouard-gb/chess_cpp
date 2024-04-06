#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector> 
#include "pawn.h"
#include "bishop.h"

class Game : public sf::Drawable
{
public:
    Game();
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    void setup();
    void setup_piece(Piece* piece);
    void select_piece(int x, int y);
    void make_move(int x, int y);
    void kill_piece_at(Square &cell);
    void move_selected_piece_to(Square &cell);
    bool is_piece_selected();

    // 0 for black, 1 for white
    int color_to_play;
    int move_counter;
    // grid has matrix notation, i.e. first indexor is row, second is column
    Square grid[8][8];
    bool is_game_over;
    Piece *selected_piece;
private:
    // First dimension is the color of the piece:
    // 0 for black, 1 for white.
    // Second dimension is the piece type:
    // 5: king, 4: Queen, 3: Rook, 2: Knight, 1: Bishop, 0: Pawn.
    std::vector<Piece*> pieces[2][1];
    std::vector<Piece*> taken_pieces[2][1];

};

#endif