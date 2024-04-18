#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector> 
#include "pawn.h"
#include "bishop.h"
#include "rook.h"
#include "queen.h"
#include "knight.h"
#include "king.h"

class ChessMove {
public:
    ChessMove(Square from, Square to, Piece& _piece, bool _en_passant = false, bool _castle = false);
    bool operator == (const ChessMove &move);
    void operator = (const ChessMove& move); 
    Square square_from;
    Square square_to;
    Piece& piece;
    bool en_passant;
    bool castle;
};

class Game : public sf::Drawable {
public:
    Game();
    Game(const Game& game);
    ~Game();
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    void select_piece(int x, int y);
    void make_move(int x, int y);
    bool is_piece_selected() const;
    bool is_game_over;

private:
    void get_selected_piece_legal_moves();
    void perform_move(Square &clicked_cell, bool is_en_passant = false, bool is_castle = false);
    void move_piece_to(Square &cell, Piece* piece);
    void take_piece_at(Square &cell);
    void check_and_add_legal_moves();
    void check_and_add_en_passant_move();
    void check_and_add_castling_moves();
    void setup();
    void setup_piece(Piece* piece);
    bool is_king_in_check(bool color) const;
    // 0 for black, 1 for white
    int color_to_play;
    int move_counter;
    // grid has matrix notation for indexers, i.e. first indexor is row, second is column
    Square grid[8][8];
    Piece *selected_piece;
    std::vector<ChessMove> selected_piece_legal_moves;

    // First dimension is the color of the piece:
    // 0 for black, 1 for white.
    // Second dimension is the piece type:
    // 5: king, 4: Queen, 3: Rook, 2: Knight, 1: Bishop, 0: Pawn.
    std::vector<Piece*> pieces[2][6];
    std::vector<Piece*> taken_pieces[2][6];
    std::vector<ChessMove> recorded_moves;

};

#endif