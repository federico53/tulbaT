#include "headers/Stats.h"

// this return true if white is doing a checkmate
bool black_checkmate(const vector<vector<char>>& board, const std::pair<int, int>& king_position) {
    try {
        if (king_position.first == 0 || king_position.first == 8 || king_position.second == 0 || king_position.second == 8) {
            return false;
        }
        std::vector<std::pair<int, int>> move_directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        for (const auto& direction : move_directions) {
            auto new_position = std::make_pair(king_position.first + direction.first, king_position.second + direction.second);
            while (new_position.first >= 0 && new_position.first <= 8 && new_position.second >= 0 && new_position.second <= 8) {
                if (board[new_position.first][new_position.second] != 'E' || is_citadel(new_position.first, new_position.second) || (new_position.first == 4 && new_position.second == 4)) {
                    break;
                }
                if (is_winning_position(new_position.first, new_position.second)) {
                    return true;
                }
                
                new_position = std::make_pair(new_position.first + direction.first, new_position.second + direction.second);
            }
        }
        return false;
    } catch (const std::exception& e) {
        Logger::error("An error occurred in black_checkmate: " + std::string(e.what()));
        throw;
    }
}



bool is_empty_and_reachable(const vector<vector<char>>& board, const std::pair<int, int>& position, const char color) {
    try {
        if (board[position.first][position.second] != 'E') {
            return false;
        }

        std::vector<std::pair<int, int>> move_directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

        // Check for the special rule
        if (is_citadel(position.first, position.second)) {
            for (const auto& direction : move_directions) {
                auto new_position = std::make_pair(position.first + direction.first, position.second + direction.second);
                while (new_position.first >= 0 && new_position.first <= 8 && new_position.second >= 0 && new_position.second <= 8) {
                    if (is_citadel(new_position.first, new_position.second)) {
                        if (board[new_position.first][new_position.second] == color) {
                            return true;
                        }
                    } else {
                        break;
                    }
                    if (board[new_position.first][new_position.second] != 'E') {
                        break;
                    }
                    new_position = std::make_pair(new_position.first + direction.first, new_position.second + direction.second);
                }
            }
        }

        for (const auto& direction : move_directions) {
            auto new_position = std::make_pair(position.first + direction.first, position.second + direction.second);
            while (new_position.first >= 0 && new_position.first <= 8 && new_position.second >= 0 && new_position.second <= 8) {
                if (board[new_position.first][new_position.second] == color) { // if the position is a piece of the color
                    return true;
                }
                if (board[new_position.first][new_position.second] != 'E') { // if the position is not empty
                    break;
                }
                if (is_citadel(new_position.first, new_position.second) || (new_position.first == 4 && new_position.second == 4)) { // if the position is a black camp or the castle
                    break;
                }
                new_position = std::make_pair(new_position.first + direction.first, new_position.second + direction.second);
            }
        }
        return false;
    } catch (const std::exception& e) {
        Logger::error("An error occurred in is_empty_and_reachable: " + std::string(e.what()));
        throw;
    }
}

bool king_adjacent_to_castle(const std::pair<int, int>& king_position) {
    try {
        return king_position == std::make_pair(3, 4) || king_position == std::make_pair(5, 4) ||
               king_position == std::make_pair(4, 3) || king_position == std::make_pair(4, 5);
    } catch (const std::exception& e) {
        Logger::error("An error occurred in king_adjacent_to_castle: " + std::string(e.what()));
        throw;
    }
}

bool king_can_be_captured_between_two_blacks(const vector<vector<char>>& board, const pair<int, int>& king_position) {
    try {
        if (king_position.first == 0 || king_position.first == 8 || king_position.second == 0 || king_position.second == 8) {
            return false;
        }
        if ((king_position.first == 4 && king_position.second == 4) || king_adjacent_to_castle(king_position)) {
            return false;
        }
        if ((board[king_position.first - 1][king_position.second] == 'B' || is_citadel(king_position.first - 1, king_position.second)) &&
            is_empty_and_reachable(board, std::make_pair(king_position.first + 1, king_position.second), 'B')) {
            return true;
        }
        if ((board[king_position.first + 1][king_position.second] == 'B' || is_citadel(king_position.first + 1, king_position.second)) &&
            is_empty_and_reachable(board, std::make_pair(king_position.first - 1, king_position.second), 'B')) {
            return true;
        }
        if ((board[king_position.first][king_position.second - 1] == 'B' || is_citadel(king_position.first, king_position.second - 1)) &&
            is_empty_and_reachable(board, std::make_pair(king_position.first, king_position.second + 1), 'B')) {
            return true;
        }
        if ((board[king_position.first][king_position.second + 1] == 'B' || is_citadel(king_position.first, king_position.second + 1)) &&
            is_empty_and_reachable(board, std::make_pair(king_position.first, king_position.second - 1), 'B')) {
            return true;
        }
        return false;
    } catch (const std::exception& e) {
        Logger::error("An error occurred in king_can_be_captured_between_two_blacks: " + std::string(e.what()));
        throw;
    }
}

bool king_adjacent_to_castle_can_be_captured(const vector<vector<char>>& board, const pair<int, int>& king_position) {
    try {
        if (king_position.first == 0 || king_position.first == 8 || king_position.second == 0 || king_position.second == 8) {
            return false;
        }
        if (!king_adjacent_to_castle(king_position)) {
            return false;
        }
        if (king_position == std::make_pair(3, 4)) {
            if ((board[3][3] == 'B' || is_citadel(3, 3)) &&
                is_empty_and_reachable(board, std::make_pair(3, 5), 'B') &&
                is_empty_and_reachable(board, std::make_pair(4, 4), 'B')) {
                return true;
            }
            if ((board[4][4] == 'B' || is_citadel(4, 4)) &&
                is_empty_and_reachable(board, std::make_pair(3, 3), 'B') &&
                is_empty_and_reachable(board, std::make_pair(3, 5), 'B')) {
                return true;
            }
            if ((board[3][5] == 'B' || is_citadel(3, 5)) &&
                is_empty_and_reachable(board, std::make_pair(3, 3), 'B') &&
                is_empty_and_reachable(board, std::make_pair(4, 4), 'B')) {
                return true;
            }
        }
        if (king_position == std::make_pair(5, 4)) {
            if ((board[5][3] == 'B' || is_citadel(5, 3)) &&
                is_empty_and_reachable(board, std::make_pair(5, 5), 'B') &&
                is_empty_and_reachable(board, std::make_pair(4, 4), 'B')) {
                return true;
            }
            if ((board[4][4] == 'B' || is_citadel(4, 4)) &&
                is_empty_and_reachable(board, std::make_pair(5, 3), 'B') &&
                is_empty_and_reachable(board, std::make_pair(5, 5), 'B')) {
                return true;
            }
            if ((board[5][5] == 'B' || is_citadel(5, 5)) &&
                is_empty_and_reachable(board, std::make_pair(5, 3), 'B') &&
                is_empty_and_reachable(board, std::make_pair(4, 4), 'B')) {
                return true;
            }
        }
        if (king_position == std::make_pair(4, 3)) {
            if ((board[3][3] == 'B' || is_citadel(3, 3)) &&
                is_empty_and_reachable(board, std::make_pair(5, 3), 'B') &&
                is_empty_and_reachable(board, std::make_pair(4, 4), 'B')) {
                return true;
            }
            if ((board[4][4] == 'B' || is_citadel(4, 4)) &&
                is_empty_and_reachable(board, std::make_pair(3, 3), 'B') &&
                is_empty_and_reachable(board, std::make_pair(5, 3), 'B')) {
                return true;
            }
            if ((board[5][3] == 'B' || is_citadel(5, 3)) &&
                is_empty_and_reachable(board, std::make_pair(3, 3), 'B') &&
                is_empty_and_reachable(board, std::make_pair(4, 4), 'B')) {
                return true;
            }
        }
        if (king_position == std::make_pair(4, 5)) {
            if ((board[3][5] == 'B' || is_citadel(3, 5)) &&
                is_empty_and_reachable(board, std::make_pair(5, 5), 'B') &&
                is_empty_and_reachable(board, std::make_pair(4, 4), 'B')) {
                return true;
            }
            if ((board[4][4] == 'B' || is_citadel(4, 4)) &&
                is_empty_and_reachable(board, std::make_pair(3, 5), 'B') &&
                is_empty_and_reachable(board, std::make_pair(5, 5), 'B')) {
                return true;
            }
            if ((board[5][5] == 'B' || is_citadel(5, 5)) &&
                is_empty_and_reachable(board, std::make_pair(3, 5), 'B') &&
                is_empty_and_reachable(board, std::make_pair(4, 4), 'B')) {
                return true;
            }
        }
        return false;
    } catch (const std::exception& e) {
        Logger::error("An error occurred in king_adjacent_to_castle_can_be_captured: " + std::string(e.what()));
        throw;
    }
}

bool king_in_the_castle_can_be_captured(const vector<vector<char>>& board, const pair<int, int>& king_position) {
    try {
        if (king_position.first == 0 || king_position.first == 8 || king_position.second == 0 || king_position.second == 8) {
            return false;
        }
        if (!(king_position.first == 4 && king_position.second == 4)) {
            return false;
        }
        if (board[3][4] == 'B' && board[4][3] == 'B' && board[4][5] == 'B' && is_empty_and_reachable(board, std::make_pair(5, 4), 'B')) {
            return true;
        }
        if (board[5][4] == 'B' && board[4][3] == 'B' && board[4][5] == 'B' && is_empty_and_reachable(board, std::make_pair(3, 4), 'B')) {
            return true;
        }
        if (board[4][3] == 'B' && board[3][4] == 'B' && board[5][4] == 'B' && is_empty_and_reachable(board, std::make_pair(4, 5), 'B')) {
            return true;
        }
        if (board[4][5] == 'B' && board[3][4] == 'B' && board[5][4] == 'B' && is_empty_and_reachable(board, std::make_pair(4, 3), 'B')) {
            return true;
        }
        return false;
    } catch (const std::exception& e) {
        Logger::error("An error occurred in king_in_the_castle_can_be_captured: " + std::string(e.what()));
        throw;
    }
}

bool white_checkmate(const vector<vector<char>>& board, const pair<int, int>& king_position) {
    try {
        return king_can_be_captured_between_two_blacks(board, king_position) ||
               king_adjacent_to_castle_can_be_captured(board, king_position) ||
               king_in_the_castle_can_be_captured(board, king_position);
    } catch (const std::exception& e) {
        Logger::error("An error occurred in white_checkmate: " + std::string(e.what()));
        throw;
    }
}

std::string format_board(const vector<vector<char>>& board) {
    // Mappa per convertire i simboli della board nei caratteri desiderati
    std::unordered_map<char, char> symbols = {
        {'W', 'W'},  // WHITE
        {'B', 'B'},  // BLACK
        {'K', 'K'},  // KING
        {'T', 'T'},  // THRONE
        {'E', ' '}   // EMPTY
    };

    // Etichette delle colonne
    std::string column_labels = "A   B   C   D   E   F   G   H   I";
    std::ostringstream board_str;

    // Etichette in alto
    board_str << "    " << column_labels << "\n";
    board_str << "  +" << std::string(9 * 4 - 1, '-') << "+\n";

    // Creazione della board
    for (int i = 0; i < 9; ++i) {
        board_str << i + 1 << " | ";  // Etichetta della riga
        for (int j = 0; j < 9; ++j) {
            // Controlla se il simbolo esiste nella mappa, altrimenti usa un carattere predefinito
            char cell_symbol = (symbols.count(board[i][j]) > 0) ? symbols[board[i][j]] : '?';
            board_str << cell_symbol << " | ";
        }
        board_str << "\n";
        board_str << "  +" << std::string(9 * 4 - 1, '-') << "+\n";
    }

    return board_str.str();
}
