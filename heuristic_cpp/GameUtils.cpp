#include "headers/GameUtils.h"

static const vector<pair<int, int>> citadels = {
    {0, 3}, {0, 4}, {0, 5}, {1, 4}, {3, 0}, {4,0}, {5,0}, {4,1},
    {3,8}, {4,8}, {5,8}, {4,7}, {8,3}, {8,4}, {8,5}, {7,4}
};

static const std::vector<std::pair<int, int>> winning_positions = {
{0, 1}, {0, 2}, {0, 6}, {0, 7}, {1, 0}, {2, 0}, {6, 0}, {7, 0}, {8, 1}, {8, 2}, {8, 6}, {8, 7}, {1, 8}, {2, 8}, {6, 8}, {7, 8} };

// PARSE MOVE

Move parseMove(const std::string& move) {
    Move res;
    res.from = {move[1] - '1', move[0] - 'a'}; 
    res.to = {move[3] - '1', move[2] - 'a'};
    return res;
}

// POSITION VALIDATION

bool is_within_bounds(int row, int col) {
    return row >= 0 && row < 9 && col >= 0 && col < 9;
}

// POSITION UTILS

bool is_citadel(int row, int col) {
    for(const auto& c : citadels){
        if(c.first == row && c.second == col){
            return true;
        }
    }
    return false;
}

bool is_winning_position(int row, int col) {
    for(const auto& w : winning_positions){
        if(w.first == row && w.second == col){
            return true;
        }
    }
    return false;
}

bool is_adjacent_to_castle(int row, int col) {
    // Verifica se la posizione è adiacente al castello
    int castle_row = 4, castle_col = 4;  // Il castello si trova nella posizione (4, 4)
    std::vector<std::pair<int, int>> adjacent_positions = {
        {castle_row + 1, castle_col},  // Sotto
        {castle_row - 1, castle_col},  // Sopra
        {castle_row, castle_col + 1},  // Destra
        {castle_row, castle_col - 1}   // Sinistra
    };

    for (const auto& pos : adjacent_positions) {
        if (pos.first == row && pos.second == col) {
            return true;
        }
    }
    return false;
}

bool is_blocking_cell(int row, int col) {
    // Verifica se la cella è una cella di blocco (come un castello o una cittadella)
    return (row == 4 && col == 4) || is_citadel(row, col);
}

bool is_king_captured(const std::vector<std::vector<char>>& board, const pair<int, int>& king_pos) {
    try {

        int king_row = king_pos.first;
        int king_col = king_pos.second;

        std::vector<std::pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};  // Down, Up, Right, Left

        // Case 1: Check if the king is in the castle
        if (king_row == 4 && king_col == 4) {
            // King must be surrounded on all four sides by black pieces to be captured
            for (const auto& dir : directions) {
                int new_row = king_row + dir.first;
                int new_col = king_col + dir.second;
                if (is_within_bounds(new_row, new_col) && board[new_row][new_col] == 'B') {
                    continue;
                }
                return false;  // If any side is not surrounded by black, king is not captured
            }
            return true;  // All sides are surrounded by black pieces
        }

        // Case 2: Check if the king is adjacent to the castle
        else if (is_adjacent_to_castle(king_row, king_col)) {
            // King is captured if surrounded on three sides by black pieces
            int black_count = 0;
            for (const auto& dir : directions) {
                int new_row = king_row + dir.first;
                int new_col = king_col + dir.second;
                if (is_within_bounds(new_row, new_col) && board[new_row][new_col] == 'B') {
                    black_count++;
                }
            }
            return black_count >= 3;
        }

        // Case 3: General capture condition for non-castle, non-adjacent cells
        else {
            // King must be surrounded on two opposite sides by black pieces or blocking cells
            if (is_within_bounds(king_row + 1, king_col) && (is_blocking_cell(king_row + 1, king_col) || board[king_row + 1][king_col] == 'B') &&
                is_within_bounds(king_row - 1, king_col) && (is_blocking_cell(king_row - 1, king_col) || board[king_row - 1][king_col] == 'B')) {
                return true;
            }

            if (is_within_bounds(king_row, king_col + 1) && (is_blocking_cell(king_row, king_col + 1) || board[king_row][king_col + 1] == 'B') &&
                is_within_bounds(king_row, king_col - 1) && (is_blocking_cell(king_row, king_col - 1) || board[king_row][king_col - 1] == 'B')) {
                return true;
            }
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error in is_king_captured: " << e.what() << std::endl;
        throw;
    }
}


// OPPOSITE TURN

char get_opposite_turn(const char &turn) {
    return (turn == 'W') ? 'B' : 'W';
}

// MOVE VALIDATION

bool is_valid_move(Move& move, const vector<vector<char>> &board, const char &color) {
    try{
        int from_row = move.from.first, from_col = move.from.second;
        int to_row = move.to.first, to_col = move.to.second;

        // Check if destination is empty
        if (board[to_row][to_col] != 'E') return false;

        // Check if it is a black piece moving within a citadel: legal, return true
        if (color == 'B' && is_citadel(from_row, from_col) && is_citadel(to_row, to_col)) {
            if (from_row == to_row) {
                int step = (to_col > from_col) ? 1 : -1;
                for (int col = from_col + step; col != to_col; col += step) {
                    if (!(board[from_row][col] == 'E' && is_citadel(from_row, col))) return false;
                }
            } else if (from_col == to_col) {
                int step = (to_row > from_row) ? 1 : -1;
                for (int row = from_row + step; row != to_row; row += step) {
                    if (!(board[row][from_col] == 'E' && is_citadel(row, from_col))) return false;
                }
            } else {
                return false;  // Diagonal move
            }
            return true;
        }

        if (!is_citadel(from_row, from_col) && is_citadel(to_row, to_col)) {
            return false;
        } // if i am not in a citadel and i am moving to a citadel

        // Check if movement is valid (straight line, no obstacles)
        if (from_row == to_row) {
            int step = (to_col > from_col) ? 1 : -1;
            for (int col = from_col + step; col != to_col; col += step) {
                if (board[from_row][col] != 'E' || (is_citadel(from_row, col) && !(is_citadel(from_row, from_col)))) return false;
            }
        } else if (from_col == to_col) {
            int step = (to_row > from_row) ? 1 : -1;
            for (int row = from_row + step; row != to_row; row += step) {
                if (board[row][from_col] != 'E' || (is_citadel(row, from_col) && !(is_citadel(from_row, from_col)))) return false;
            }
        } else {
            return false; // Diagonal moves not allowed
        }

        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error in is_valid_move: " << e.what() << std::endl;
        throw;
    }
}

// MOVE GENERATION

vector<Move> generate_all_possible_moves(const vector<vector<char>> &board, const char &color) {

    try{
        vector<Move> moves;
        vector<pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

        for (int row = 0; row < board.size(); ++row) {
            for (int col = 0; col < board[row].size(); ++col) {
                if ((color == 'W' && (board[row][col] == 'W' || board[row][col] == 'K')) ||
                    (color == 'B' && board[row][col] == 'B')) {
                    for (auto &[dr, dc] : directions) {
                        for (int i = 1; i < board.size(); ++i) {
                            int new_row = row + dr * i, new_col = col + dc * i;
                            if (!is_within_bounds(new_row, new_col) || board[new_row][new_col] != 'E') break;

                            pair<int, int> from = {row, col}, to = {new_row, new_col};
                            Move m;
                            m.from = from;
                            m.to = to;

                            if (is_valid_move(m, board, color)) {
                                moves.push_back(m);
                            }
                        }
                    }
                }
            }
        }
        return moves;

    } catch (const std::exception& e) {
        std::cerr << "Error in generate_all_possible_moves: " << e.what() << std::endl;
        throw;

    }
}

// GAME OVER

char is_game_over(const std::vector<std::vector<char>>& board) {
    try {
        // 1. Check if the king is in an escape cell (white wins)
        pair<int, int> king_position;
        bool found = false;
        for(int i = 0; i < 9 && !found; i++){
            for(int j = 0; j < 9 && !found; j++){
                if(board[i][j] == 'K'){
                    king_position = std::make_pair(i, j);
                    found = true;
                }
            }
        }
        
        for(const auto& w : winning_positions){
            if(w.first == king_position.first && w.second == king_position.second){
                return 'W';
            }
        }
        
        // 2. Check if the king is captured (black wins)
        if (is_king_captured(board, king_position)) {
            return 'B';  // Black player wins
        }

        // CORNER CASE VERIFY IF IS COMPUTATIONAL EXPENSIVE
        // // 3. Check if a player has no possible moves (the other player wins)
        // if (generate_all_possible_moves(board, 'W').empty()) {
        //     return "black";  // Black player wins
        // }
        // if (generate_all_possible_moves(board, 'B').empty()) {
        //     return "white";  // White player wins
        // }
        
        return 'N';  // No winner yet
    } catch (const std::exception& e) {
        std::cerr << "Error in is_game_over: " << e.what() << std::endl;
        throw;
    }
}


// APPLY MOVE

bool is_enemy(const char& enemy, const char& of){
    if(of == 'B'){
        return enemy == 'W';
    } else if(of == 'W'){
        return enemy == 'B';
    } else if(of == 'K'){
        return enemy == 'B';
    }
    return false;
}

bool is_allie(const char& allie, const char& of, const std::vector<std::vector<char>>& board){
    pair<int, int> king_position;
    bool found = false;
    for(int i = 0; i < 9 && !found; i++){
        for(int j = 0; j < 9 && !found; j++){
            if(board[i][j] == 'K'){
                king_position = std::make_pair(i, j);
                found = true;
            }
        }
    }

    if(of == 'B'){
        return allie == 'B' || allie == 'T' || (allie == 'K' && king_position == std::make_pair(4, 4));
    } else if(of == 'W'){
        return allie == 'W' || allie == 'K' || allie == 'T';
    } else if(of == 'K'){
        return allie == 'W' || allie == 'T';
    }
    return false;
}


std::vector<std::vector<char>> apply_move(const std::vector<std::vector<char>>& board, const Move& move) {

    try {
        // Decomponiamo la mossa
        auto[from_pos, to_pos] = move;
        int from_row = from_pos.first, from_col = from_pos.second;
        int to_row = to_pos.first, to_col = to_pos.second;

        // Creiamo una nuova scacchiera copiando la vecchia
        std::vector<std::vector<char>> new_board = board;
        
        // Muoviamo il pezzo sulla nuova scacchiera
        new_board[to_row][to_col] = new_board[from_row][from_col];
        new_board[from_row][from_col] = 'E';

        // Direzioni in cui un pezzo può catturare
        std::vector<std::pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

        // Controlliamo se un pezzo è stato catturato
        for (const auto& [dr, dc] : directions) {
            int check_row = to_row + dr;
            int check_col = to_col + dc;
            int capture_row = to_row + 2*dr;
            int capture_col = to_col + 2*dc;

            // Verifica se la posizione è valida e se c'è un pezzo nemico da catturare
            if (is_within_bounds(capture_row, capture_col) && is_enemy(new_board[check_row][check_col], new_board[to_row][to_col]) && 
                (is_allie(new_board[capture_row][capture_col], new_board[to_row][to_col], board) || is_citadel(capture_row, capture_col) && !is_citadel(check_row, check_col))){
                    if(new_board[check_row][check_col] != 'K'){
                        new_board[check_row][check_col] = 'E';
                    }
            }
        }

        return new_board;

    } catch (const std::exception& e) {
        std::cerr << "Error in apply_move: " << e.what() << std::endl;
        throw; // Rilancia l'eccezione
    }
}


// BOARD REPRESENTATION

std::string format_board(const std::vector<std::vector<std::string>>& board) {
    // Rappresentazione simbolica della scacchiera
    std::map<std::string, char> symbols = {
        {"WHITE", 'W'},
        {"BLACK", 'B'},
        {"KING", 'K'},
        {"THRONE", 'T'},
        {"EMPTY", ' '}
    };

    // Etichette delle colonne
    std::string column_labels = "A B C D E F G H I";
    std::string board_str = "    ";
    
    // Aggiungi le etichette delle colonne
    std::istringstream col_stream(column_labels);
    std::string col;
    while (col_stream >> col) {
        board_str += col + "   ";
    }
    board_str += "\n";
    board_str += "  +" + std::string(4 * 9, '-') + "+\n";

    // Aggiungi le righe della scacchiera
    for (int i = 0; i < board.size(); ++i) {
        board_str += std::to_string(i + 1) + " | ";
        for (int j = 0; j < board[i].size(); ++j) {
            board_str += symbols[board[i][j]] + " | ";
        }
        board_str += "\n";
        board_str += "  +" + std::string(4 * 9, '-') + "+\n";
    }

    return board_str;
}




