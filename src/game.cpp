#include <iostream>
#include <cmath>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <SFML/Graphics.hpp>
#include "game.h"

Game::Game(){
    is_game_over = false;
    move_counter = 0;
    color_to_play = 1;
    selected_piece = nullptr;
    // grid
    for (int y = 0; y < 8; y++){
        for (int x = 0; x < 8; x++){
            grid[y][x] = Square(x, y);
        }
    }
    setup();
}

Game::Game(const Game& game){
    is_game_over = game.is_game_over;
    move_counter = game.move_counter;
    color_to_play = game.color_to_play;
    selected_piece = nullptr;
    // grid
    for (int y = 0; y < 8; y++){
        for (int x = 0; x < 8; x++){
            grid[y][x] = Square(x, y);
        }
    }
    for (int color = 0; color < 2; color++) {
        for (int p_type_index = 0; p_type_index < 6; p_type_index++) {
            for (int i = 0; i < game.pieces[color][p_type_index].size(); i++){
                setup_piece(game.pieces[color][p_type_index][i]->Clone());
                if (game.selected_piece == game.pieces[color][p_type_index][i]){
                    selected_piece = pieces[color][p_type_index].back();
                }
            }
        }
    }
}

Game::~Game(){
    delete selected_piece;
    for (int color = 0; color < 2; color++) {
        for (int p_type_index = 0; p_type_index < 6; p_type_index++) {
            for (int i = 0; i < pieces[color][p_type_index].size(); i++){
                delete pieces[color][p_type_index][i];
            }
            for (int i = 0; i < taken_pieces[color][p_type_index].size(); i++){
                delete taken_pieces[color][p_type_index][i];
            }
        }
    }
}

void Game::setup_piece(Piece* piece){
    pieces[piece->color][piece->value].push_back(piece);
    grid[piece->y][piece->x].set_occupant(pieces[piece->color][piece->value].back());
}

void Game::setup(){
    //// pieces
    for (int color = 0; color < 2; color++){
        int row_index = (color == 0) ? 0 : 7;
        //bishops
        setup_piece(new Bishop(color, 2, row_index));
        setup_piece(new Bishop(color, 5, row_index));
        //rooks
        setup_piece(new Rook(color, 0, row_index));
        setup_piece(new Rook(color, 7, row_index));
        //knight
        setup_piece(new Knight(color, 1, row_index));
        setup_piece(new Knight(color, 6, row_index));
        //queens
        setup_piece(new Queen(color, 3, row_index));
        //kings
        setup_piece(new King(color, 4, row_index));
        //pawns
        for (int i = 0; i < 8; i++){
            row_index = (color == 0) ? 1 : 6;
            setup_piece(new Pawn(color, i, row_index));
        }
    }
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.clear();
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            target.draw(grid[i][j].square);
            if (grid[i][j].is_occupied()){
                target.draw(grid[i][j].occupant->piece);
            }
        }
    }
    if (selected_piece != nullptr){
        for (int i = 0; i < selected_piece_legal_moves.size(); i++){
            const Square& move_square = selected_piece_legal_moves[i];
            target.draw(grid[move_square.y][move_square.x].circle_move_indicator);
        }
    }
}

bool Game::is_piece_selected() const {
    return selected_piece != nullptr;
}

bool Game::is_king_in_check(bool color) const {
    bool king_is_attacked;
    Square square_of_king = Square(pieces[color][5][0]->x, pieces[color][5][0]->y);

    for (int p_type_index = 0; p_type_index < 6; p_type_index++){
        for (int p_index = 0; p_index < pieces[!color][p_type_index].size(); p_index++){
            std::vector<Square> attacked_squares = pieces[!color][p_type_index][p_index]->get_attacked_squares(grid);
            king_is_attacked = std::count(attacked_squares.begin(), attacked_squares.end(), square_of_king) > 0;
            if (king_is_attacked){
                return true;
            }
        }
    }
    return false;
}

void Game::select_piece(int x, int y){
    Square& clicked_cell = grid[y][x];

    if (!clicked_cell.is_occupied()){
        selected_piece = nullptr;
        return;
    }

    if (color_to_play != clicked_cell.occupant->color){
        selected_piece = nullptr;
        return;
    }

    selected_piece = clicked_cell.occupant;
    // get legal moves
    std::vector<Square> possible_moves = selected_piece->get_possible_squares(grid);
    int i = 0;
    while (i < possible_moves.size()) {
        Game chess = Game(*this);
        chess.make_move(chess.grid[possible_moves[i].y][possible_moves[i].x]);
        if (chess.is_king_in_check(color_to_play)){
            possible_moves.erase(possible_moves.begin() + i);
        } else {
            i++;
        }
    }
    selected_piece_legal_moves = possible_moves;
    return;
}

void Game::check_and_make_move(int x, int y){
    // piece must have been selected already by here
    Square& clicked_cell = grid[y][x];

    // check if clicked on another own piece.
    if (clicked_cell.is_occupied()){
        if (color_to_play == clicked_cell.occupant->color){
            select_piece(x, y);
            return;
        }
    }
    //// check move validity
    bool is_clicked_cell_valid = std::count(selected_piece_legal_moves.begin(), selected_piece_legal_moves.end(), clicked_cell) > 0;
    if (!is_clicked_cell_valid){
        selected_piece = nullptr;
        return;
    }
    bool is_clicked_cell_legal = std::count(selected_piece_legal_moves.begin(), selected_piece_legal_moves.end(), clicked_cell) > 0;
    if (!is_clicked_cell_legal){
        return;
    }

    make_move(clicked_cell);
    move_counter += 1;
    color_to_play = !color_to_play;
}

void Game::make_move(Square &clicked_cell){

    // kill piece if clicked cell is occupied by a piece of opposite color
    if (clicked_cell.is_occupied()){
        kill_piece_at(clicked_cell);
    }

    // handle promotion

    // move the selected piece to that clicked cell
    move_selected_piece_to(clicked_cell);
}

void Game::kill_piece_at(Square &cell) {
    Piece*& piece_killed = cell.occupant;
    taken_pieces[!color_to_play][piece_killed->value].push_back(piece_killed);
    piece_killed->dies();
    std::vector<Piece*>& pieces_of_value = pieces[!color_to_play][piece_killed->value];
    pieces_of_value.erase(
        std::remove_if(
            pieces_of_value.begin(), 
            pieces_of_value.end(),
            [](Piece* const & piece) { return !piece->is_alive;}
        ), 
        pieces_of_value.end()
    );
    cell.set_occupant(nullptr);
}

void Game::move_selected_piece_to(Square &cell){
    grid[selected_piece->y][selected_piece->x].set_occupant(nullptr);
    cell.set_occupant(selected_piece);
    selected_piece->set_position(cell.x, cell.y);
    selected_piece = nullptr;
}