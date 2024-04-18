#include <iostream>
#include <cmath>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <SFML/Graphics.hpp>
#include "game.h"

ChessMove::ChessMove(Square from, Square to, Piece& _piece, bool _en_passant, bool _castle): 
    square_from(from), square_to(to), piece(_piece), en_passant(_en_passant), castle(_castle) {}

bool ChessMove::operator == (const ChessMove& move) {
    if (
        square_from == move.square_from 
        and square_to == move.square_to 
        and piece == move.piece
    ) {
        return true;
    }
    return false;
}

void ChessMove::operator = (const ChessMove& move) { 
        square_from = Square(move.square_from.x, move.square_from.y, false);
        square_to = Square(move.square_to.x, move.square_to.y, false);
        piece = move.piece;
        en_passant = move.en_passant;
        castle = move.castle;
}

Game::Game(){
    is_game_over = false;
    move_counter = 0;
    color_to_play = 1;
    selected_piece = nullptr;
    recorded_moves = {};
    selected_piece_legal_moves = {};

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
    recorded_moves = game.recorded_moves;
    selected_piece_legal_moves.clear();
    selected_piece = nullptr;

    // grid
    for (int y = 0; y < 8; y++){
        for (int x = 0; x < 8; x++){
            grid[y][x] = Square(x, y);
        }
    }
    for (int color : {0, 1}){
        for (auto piece_type_vec : game.pieces[color]){
            for (auto piece: piece_type_vec){
                setup_piece(piece->Clone());
                if (piece == game.selected_piece){
                    selected_piece = pieces[color][piece->value].back();
                }
            }
        }
    }
}

Game::~Game(){
    for (int color : {0, 1}){
        for (int p_type_index = 0; p_type_index < 6; p_type_index++) {
            for (auto piece: pieces[color][p_type_index]){
                delete piece;
            }
            for (auto taken_piece: taken_pieces[color][p_type_index]){
                delete taken_piece;
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
    for (int color : {0, 1}){
        int row_index = (color == 0) ? 0 : 7;
        //bishops
        setup_piece(new Bishop(color, 2, row_index));
        setup_piece(new Bishop(color, 5, row_index));
        //rooks
        setup_piece(new Rook(color, 0, row_index, false));
        setup_piece(new Rook(color, 7, row_index, true));
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
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            target.draw(grid[i][j].square);
            if (grid[i][j].is_occupied()){
                target.draw(grid[i][j].occupant->piece);
            }
        }
    }
    if (selected_piece != nullptr){
        for (auto legal_move: selected_piece_legal_moves){
            target.draw(grid[legal_move.square_to.y][legal_move.square_to.x].circle_move_indicator);
        }
    }
}

bool Game::is_piece_selected() const {
    return selected_piece != nullptr;
}

bool Game::is_king_in_check(bool color) const {
    bool king_is_attacked;
    Square square_of_king = Square(pieces[color][5][0]->x, pieces[color][5][0]->y);

    for (auto piece_type_vec : pieces[!color]){
        for (auto piece: piece_type_vec){
            std::vector<Square> attacked_squares = piece->get_attacked_squares(grid);
            king_is_attacked = std::count(attacked_squares.begin(), attacked_squares.end(), square_of_king) > 0;
            if (king_is_attacked){
                return true;
            }
        }
    }
    return false;
}

void Game::select_piece(int x, int y){
    Square& clicked_square = grid[y][x];

    if (!clicked_square.is_occupied()){
        selected_piece = nullptr;
        return;
    }

    if (color_to_play != clicked_square.occupant->color){
        selected_piece = nullptr;
        return;
    }

    selected_piece = clicked_square.occupant;
    get_selected_piece_legal_moves();
}

void Game::get_selected_piece_legal_moves(){
    // check and get legal moves as per checks rules
    check_and_add_legal_moves();
    // check and get en-passant move
    check_and_add_en_passant_move();
    // check and get castling moves
    check_and_add_castling_moves();

}

void Game::check_and_add_legal_moves(){
    std::vector<Square> possible_squares = selected_piece->get_possible_squares(grid);
    int i = 0;
    while (i < possible_squares.size()) {
        Game chess = Game(*this);
        chess.perform_move(chess.grid[possible_squares[i].y][possible_squares[i].x]);
        if (!(chess.is_king_in_check(color_to_play))){
            selected_piece_legal_moves.push_back(
                ChessMove(
                    Square(selected_piece->x, selected_piece->y, false),
                    Square(possible_squares[i].x, possible_squares[i].y, false),
                    *selected_piece
                )
            );
        }
        i++;
    }
}

void Game::check_and_add_en_passant_move(){
    if (selected_piece->value == 0 and move_counter > 0){
        ChessMove& last_move = recorded_moves.back();
        if (
            last_move.piece.value == 0 
            and std::abs(last_move.square_from.y - last_move.square_to.y) == 2
            and selected_piece->y == last_move.square_to.y
        ){
            Game chess = Game(*this);
            int y_pos = last_move.square_to.y + ((color_to_play == 0) ? 1 : -1);
            int x_pos = last_move.square_to.x;
            chess.perform_move(chess.grid[y_pos][x_pos], true);
            if (!chess.is_king_in_check(color_to_play)){
                selected_piece_legal_moves.push_back(
                    ChessMove(
                        Square(selected_piece->x, selected_piece->y, false),
                        Square(x_pos, y_pos, false),
                        *selected_piece,
                        true
                    )
                );
            }
        }
    }
}

void Game::check_and_add_castling_moves(){
    bool is_king_selected = selected_piece->value == 5;
    if (!is_king_selected) {
        return;
    } 

    King* selected_king = dynamic_cast<King*>(selected_piece);
    std::vector<Rook*> rooks;
    for (auto rook: pieces[color_to_play][3]) {
        rooks.push_back(dynamic_cast<Rook*>(rook));
    }
    if(
        selected_king->has_moved_once
        or rooks.empty()
        or is_king_in_check(color_to_play)
    ){
        return;
    }

    int row_index = (color_to_play == 0) ? 0 : 7;
    // check king-side castle
    auto iterator = std::find_if(
        rooks.begin(),
        rooks.end(), 
        [](Rook* const & piece) 
        {return piece->is_king_side_rook;}
    );
    if (iterator != rooks.end()){
        Rook* king_side_rook = dynamic_cast<Rook*>(*iterator);
        if(
            !king_side_rook->has_moved_once
            and !grid[row_index][5].is_occupied()
            and !grid[row_index][6].is_occupied()
        ){
            for (int x_coord: {5, 6}){
                Game chess = Game(*this);
                chess.perform_move(chess.grid[row_index][x_coord]);
                if (chess.is_king_in_check(color_to_play)){
                    break;
                }
            }
            selected_piece_legal_moves.push_back(
                ChessMove(
                    Square(selected_piece->x, selected_piece->y, false),
                    Square(6, row_index, false),
                    *selected_piece,
                    false,
                    true
                )
            );
        }
    }

    auto iterator_2 = std::find_if(
        rooks.begin(),
        rooks.end(), 
        [](Rook* const & piece) 
        {return !piece->is_king_side_rook;}
    );
    if (iterator_2 != rooks.end()){
        Rook* queen_side_rook = dynamic_cast<Rook*>(*iterator_2);
        if(
            !queen_side_rook->has_moved_once
            and !grid[row_index][1].is_occupied()
            and !grid[row_index][2].is_occupied()
            and !grid[row_index][3].is_occupied()
        ){
            for (int x_coord: {1, 2, 3}){
                Game chess = Game(*this);
                chess.perform_move(chess.grid[row_index][x_coord]);
                if (chess.is_king_in_check(color_to_play)){
                    break;
                }
            }
            selected_piece_legal_moves.push_back(
                ChessMove(
                    Square(selected_piece->x, selected_piece->y, false),
                    Square(2, row_index, false),
                    *selected_piece,
                    false,
                    true
                )
            );
        }
    }
}

void Game::make_move(int x, int y){
    // piece must have been selected already by here
    Square& clicked_square = grid[y][x];

    // check if clicked on another own piece.
    if (clicked_square.is_occupied()){
        if (color_to_play == clicked_square.occupant->color){
            selected_piece_legal_moves.clear();
            select_piece(x, y);
            return;
        }
    }

    ChessMove clicked_move = ChessMove(
        Square(selected_piece->x, selected_piece->y, false),
        Square(x, y, false),
        *selected_piece

    );
    bool move_found = false;
    for (auto move: selected_piece_legal_moves) {
        if (move == clicked_move){
            move_found = true;
            clicked_move = move;
            break;
        }
    }
    if (!move_found){
        return;
    }

    // make the move
    perform_move(clicked_square, clicked_move.en_passant, clicked_move.castle);

    // update game state
    recorded_moves.push_back(clicked_move);
    selected_piece_legal_moves.clear();
    selected_piece = nullptr;
    move_counter += 1;
    color_to_play = !color_to_play;
}

void Game::perform_move(Square &clicked_square, bool is_en_passant, bool is_castle){
    // kill piece if clicked cell is occupied by a piece of opposite color
    if (clicked_square.is_occupied()){
        take_piece_at(clicked_square);
    }

    if (is_en_passant){
        ChessMove& last_move = recorded_moves.back();
        take_piece_at(grid[last_move.square_to.y][last_move.square_to.x]);
    }

    if (is_castle){
        bool king_side = clicked_square.x == 6;
        std::vector<Rook*> rooks;
        for (auto rook: pieces[color_to_play][3]) {
            rooks.push_back(dynamic_cast<Rook*>(rook));
        }
        auto iterator = std::find_if(
            rooks.begin(),
            rooks.end(), 
            [king_side](Rook* const & piece) 
            {return piece->is_king_side_rook == king_side;}
        );
        Rook* concerned_rook = dynamic_cast<Rook*>(*iterator);
        move_piece_to(grid[clicked_square.y][(king_side) ? 5 : 3], concerned_rook);
    }

    // move the selected piece to the clicked cell
    move_piece_to(clicked_square, selected_piece);
}

void Game::move_piece_to(Square &square, Piece* piece){
    grid[piece->y][piece->x].set_occupant(nullptr);
    square.set_occupant(piece);
    piece->set_position(square.x, square.y);
}

void Game::take_piece_at(Square &square) {
    Piece*& piece_killed = square.occupant;
    taken_pieces[!color_to_play][piece_killed->value].push_back(piece_killed);
    piece_killed->dies();
    std::vector<Piece*>& pieces_of_value = pieces[!color_to_play][piece_killed->value];
    pieces_of_value.erase(
        std::remove_if(
            pieces_of_value.begin(), 
            pieces_of_value.end(),
            [](Piece* const & piece) {return !piece->is_alive;}
        ),
        pieces_of_value.end()
    );
    square.set_occupant(nullptr);
}
