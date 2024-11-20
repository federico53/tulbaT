#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include "Logger.h"
#include "MinMax.h"
#include "Stats.h"

using namespace std;

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

bool is_blocking_cell(int row, int col, const std::vector<std::vector<std::string>>& board) {
    // Verifica se la cella è una cella di blocco (come un castello o una cittadella)
    return board[row][col] == "CASTLE" || board[row][col] == "CITADEL";
}

bool is_king_captured(const std::vector<std::vector<std::string>>& board) {
    try {
        auto king_pos = get_king_position(board);
        if (king_pos.empty()) {
            return false;  // No king found, so it's not captured
        }

        int king_row = king_pos.first;
        int king_col = king_pos.second;

        std::vector<std::pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};  // Down, Up, Right, Left

        // Case 1: Check if the king is in the castle
        if (is_castle(king_row, king_col)) {
            // King must be surrounded on all four sides by black pieces to be captured
            for (const auto& dir : directions) {
                int new_row = king_row + dir.first;
                int new_col = king_col + dir.second;
                if (is_within_bounds(new_row, new_col) && board[new_row][new_col] == "BLACK") {
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
                if (is_within_bounds(new_row, new_col) && board[new_row][new_col] == "BLACK") {
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
                    (board[new_row][new_col] == "BLACK" || is_blocking_cell(new_row, new_col, board))) {
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

string get_opposite_turn(const string &turn) {
    return (turn == "white") ? "black" : "white";
}

// MOVE VALIDATION

bool is_valid_move(pair<int, int> from, pair<int, int> to, const vector<vector<string>> &board, const string &color) {
    int from_row = from.first, from_col = from.second;
    int to_row = to.first, to_col = to.second;

    // Check if destination is empty
    if (board[to_row][to_col] != "EMPTY") return false;

    // Check if movement is valid (straight line, no obstacles)
    if (from_row == to_row) {
        int step = (to_col > from_col) ? 1 : -1;
        for (int col = from_col + step; col != to_col; col += step) {
            if (board[from_row][col] != "EMPTY") return false;
        }
    } else if (from_col == to_col) {
        int step = (to_row > from_row) ? 1 : -1;
        for (int row = from_row + step; row != to_row; row += step) {
            if (board[row][from_col] != "EMPTY") return false;
        }
    } else {
        return false; // Diagonal moves not allowed
    }

    return true;
}