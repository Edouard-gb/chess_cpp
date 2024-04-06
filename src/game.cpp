#include <iostream>
#include <cmath>
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

void Game::setup_piece(Piece* piece){
    pieces[piece->color][piece->value].push_back(piece);
    grid[piece->y][piece->x].set_occupant(pieces[piece->color][piece->value].back());
}

void Game::setup(){
    //// pieces
    // pawns
    for (int color = 0; color < 2; color++){
        for (int i = 0; i < 8; i++){
            int row_index = (color == 0) ? 1 : 6;
            setup_piece(new Pawn(color, i, row_index));
        }
    }
    //bishops
    for (int color = 0; color < 2; color++){
        int row_index = (color == 0) ? 0 : 7;
        setup_piece(new Bishop(color, 2, row_index));
        setup_piece(new Bishop(color, 5, row_index));
    }
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const{
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
}

bool Game::is_piece_selected(){
    return selected_piece != nullptr;
}

void Game::select_piece(int x, int y){
    Square &clicked_cell = grid[y][x];

    if (!clicked_cell.is_occupied()){
        selected_piece = nullptr;
        return;
    }

    if (color_to_play != clicked_cell.occupant->color){
        selected_piece = nullptr;
        return;
    }

    selected_piece = clicked_cell.occupant;
    return;
}

void Game::make_move(int x, int y){
    // piece must have been selected already by here
    Square& clicked_cell = grid[y][x];
    // check clicking on another own piece.
    if (clicked_cell.is_occupied()){
        if (color_to_play == clicked_cell.occupant->color){
            selected_piece = clicked_cell.occupant;
            return;
        }
    }

    // check the clicked cell is part of possible moves
    std::vector<Square> possible_squares = selected_piece->get_possible_squares(grid);
    bool is_valid = std::count(possible_squares.begin(), possible_squares.end(), clicked_cell) > 0;

    if (!is_valid){
        selected_piece = nullptr;
        return;
    }

    // kill piece if clicked cell is occupied by a piece of opposite color
    if (clicked_cell.is_occupied()){
        kill_piece_at(clicked_cell);
    }
    // move the selected piece to that clicked cell
    move_selected_piece_to(clicked_cell);

    color_to_play = !color_to_play;
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
//    Piece* piece_to_move = selected_piece;
    grid[selected_piece->y][selected_piece->x].set_occupant(nullptr);
    cell.set_occupant(selected_piece);
    selected_piece->set_position(cell.x, cell.y);
    selected_piece = nullptr;
}