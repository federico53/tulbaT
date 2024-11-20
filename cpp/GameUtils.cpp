#include "GameUtils.h"

// CONSTANTS UTILS

const vector<pair<int, int>> winning_positions = {
    {0, 1}, {0, 2}, {0, 6}, {0, 7}, {1, 0}, {1, 8}, {2, 0}, {2, 8},
    {6, 0}, {6, 8}, {7, 0}, {7, 8}, {8, 1}, {8, 2}, {8, 6}, {8, 7}
};

// PARSE MOVE

Move parseMove(const std::string& move) {
    Move res;
    res.from = {move[0] - 'a', move[1] - '1'};
    res.to = {move[2] - 'a', move[3] - '1'};
    return res;
}

// POSITION VALIDATION

bool is_within_bounds(int row, int col) {
    return row >= 0 && row < 9 && col >= 0 && col < 9;
}

// POSITION UTILS

bool is_castle(int row, int col) {
    // Verifica se la posizione è il castello (trono)
    int castle_row = 4, castle_col = 4;  // Il castello si trova al centro della scacchiera
    return (row == castle_row && col == castle_col);
}

bool is_citadel(int row, int col) {
    const vector<pair<int, int>> citadels = {
        {0, 3}, {0, 4}, {0, 5}, {1, 4}, {3, 0}, {3, 1}, {3, 2},
        {4, 0}, {4, 2}, {4, 6}, {4, 8}, {5, 0}, {8, 3}, {8, 5}
    };
    for(const auto& c : citadels){
        if(c.first == row && c.second == col){
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
    return is_castle(row, col) || is_citadel(row, col);
}

bool is_king_captured(const std::vector<std::vector<char>>& board) {
    try {
        auto king_pos = get_king_position(board);

        int king_row = king_pos.first;
        int king_col = king_pos.second;

        std::vector<std::pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};  // Down, Up, Right, Left

        // Case 1: Check if the king is in the castle
        if (is_castle(king_row, king_col)) {
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
            // King must be surrounded on all four sides by black pieces or blocking cells
            for (const auto& dir : directions) {
                int new_row = king_row + dir.first;
                int new_col = king_col + dir.second;
                if (is_within_bounds(new_row, new_col) &&
                    (board[new_row][new_col] == 'B' || is_blocking_cell(new_row, new_col))) {
                    continue;
                }
                return false;  // If any side is not surrounded by black or blocking cells, king is not captured
            }
            return true;  // All sides are surrounded by black pieces or blocking cells
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
    int from_row = move.from.first, from_col = move.from.second;
    int to_row = move.to.first, to_col = move.to.second;

    // Check if destination is empty
    if (board[to_row][to_col] != 'O') return false;

    // Check if movement is valid (straight line, no obstacles)
    if (from_row == to_row) {
        int step = (to_col > from_col) ? 1 : -1;
        for (int col = from_col + step; col != to_col; col += step) {
            if (board[from_row][col] != 'O') return false;
        }
    } else if (from_col == to_col) {
        int step = (to_row > from_row) ? 1 : -1;
        for (int row = from_row + step; row != to_row; row += step) {
            if (board[row][from_col] != 'O') return false;
        }
    } else {
        return false; // Diagonal moves not allowed
    }

    return true;
}

// MOVE GENERATION

vector<Move> generate_all_possible_moves(const vector<vector<char>> &board, const char &color) {
    vector<Move> moves;
    vector<pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    for (int row = 0; row < board.size(); ++row) {
        for (int col = 0; col < board[row].size(); ++col) {
            if ((color == 'W' && (board[row][col] == 'W' || board[row][col] == 'K')) ||
                (color == 'B' && board[row][col] == 'B')) {
                for (auto &[dr, dc] : directions) {
                    for (int i = 1; i < board.size(); ++i) {
                        int new_row = row + dr * i, new_col = col + dc * i;
                        if (!is_within_bounds(new_row, new_col) || board[new_row][new_col] != 'O') break;

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
}

// GAME OVER

std::string is_game_over(const std::vector<std::vector<char>>& board) {
    try {
        // 1. Check if the king is in an escape cell (white wins)
        auto king_position = get_king_position(board);
        if (std::find(winning_positions.begin(), winning_positions.end(), king_position) != winning_positions.end()) {
            return "white";  // White player wins
        }
        
        // 2. Check if the king is captured (black wins)
        if (is_king_captured(board)) {
            return "black";  // Black player wins
        }

        // 3. Check if a player has no possible moves (the other player wins)
        if (generate_all_possible_moves(board, 'W').empty()) {
            return "black";  // Black player wins
        }
        if (generate_all_possible_moves(board, 'B').empty()) {
            return "white";  // White player wins
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error in is_game_over: " << e.what() << std::endl;
        throw;
    }
    return "";  // No winner yet
}


// APPLY MOVE

std::vector<std::vector<char>> apply_move(
    const std::vector<std::vector<char>>& board, 
    Move& move) {

    try {
        // Decomponiamo la mossa
        auto[from_pos, to_pos] = move;
        int from_row = from_pos.first, from_col = from_pos.second;
        int to_row = to_pos.first, to_col = to_pos.second;

        // Creiamo una nuova scacchiera copiando la vecchia
        std::vector<std::vector<char>> new_board = board;
        
        // Muoviamo il pezzo sulla nuova scacchiera
        new_board[to_row][to_col] = new_board[from_row][from_col];
        new_board[from_row][from_col] = 'O';

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
                (is_allie(new_board[capture_row][capture_col], new_board[to_row][to_col]) || is_citadel(capture_row, capture_col) && !is_citadel(check_row, check_col))){
                    if(new_board[check_row][check_col] != 'K'){
                        new_board[check_row][check_col] = 'O';
                    }
            }
        }

        return new_board;

    } catch (const std::exception& e) {
        std::cerr << "Error in apply_move: " << e.what() << std::endl;
        throw; // Rilancia l'eccezione
    }
}

bool is_enemy(const char& enemy, const char& of){
    if(of == 'B'){
        return enemy == 'W';
    } else if(of == 'W'){
        return enemy == 'B';
    } else if(of == 'K'){
        return enemy == 'B';
    }
}

bool is_allie(const char& allie, const char& of){
    if(of == 'B'){
        return allie == 'B' || allie == 'T';
    } else if(of == 'W'){
        return allie == 'W' || allie == 'K' || allie == 'T';
    } else if(of == 'K'){
        return allie == 'W' || allie == 'T';
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




