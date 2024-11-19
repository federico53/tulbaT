#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include "Logger.h"

using namespace std;

enum Piece { EMPTY, WHITE, BLACK, KING, THRONE };

vector<vector<int>> king_winning_direction_heatmap = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 2, 3, 1, 0, 1, 3, 2, 0},
    {0, 3, 4, 2, 2, 2, 4, 3, 0},
    {0, 1, 2, 0, 0, 0, 2, 1, 0},
    {0, 0, 2, 0, 0, 0, 2, 0, 0},
    {0, 1, 2, 0, 0, 0, 2, 1, 0},
    {0, 3, 4, 2, 2, 2, 4, 3, 0},
    {0, 2, 3, 1, 0, 1, 3, 2, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0}
};

vector<pair<int, int>> black_camps_positions = {
    {0, 3}, {0, 4}, {0, 5}, {1, 4}, {3, 0}, {3, 1}, {3, 2}, {4, 0}, {4, 2}, {4, 6}, {4, 8}, {5, 0}
};

pair<int, int> castle_position = {4, 4};

const std::vector<std::pair<int, int>> winning_positions = {
    {0, 1}, {0, 2}, {0, 6}, {0, 7}, {1, 0}, {2, 0}, {6, 0}, {7, 0}, {8, 1}, {8, 2}, {8, 6}, {8, 7}, {1, 8}, {2, 8}, {6, 8}, {7, 8}
};

int count_pieces(const std::vector<std::vector<std::string>>& board, const std::string& player) {
    try {
        int count = 0;
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (board[i][j] == player) {
                    ++count;
                }
            }
        }
        return count;
    } catch (const std::exception& e) {
        cout << "An error occurred in count_pieces: " << e.what() << endl;
        // Logger::log("An error occurred in count_pieces: " + std::string(e.what()));
        throw;
    }
}

int count_white_starting_position(const std::vector<std::vector<std::string>>& board) {
    try {
        int count = 0;
        for (int i = 2; i < 7; ++i) {
            for (int j = 2; j < 7; ++j) {
                if (board[i][j] == "WHITE") {
                    ++count;
                }
            }
        }
        return count;
    } catch (const std::exception& e) {
        cout << "An error occurred in count_white_starting_position: " << e.what() << endl;
        // Logger::log("An error occurred in count_white_starting_position: " + std::string(e.what()));
        throw;
    }
}

int count_black_starting_position(const std::vector<std::vector<std::string>>& board) {
    try {
        int count = 0;
        for (int i = 3; i < 6; ++i) {
            if (board[0][i] == "BLACK") {
                ++count;
            }
        }
        for (int i = 3; i < 6; ++i) {
            if (board[8][i] == "BLACK") {
                ++count;
            }
        }
        for (int i = 3; i < 6; ++i) {
            if (board[i][0] == "BLACK") {
                ++count;
            }
        }
        for (int i = 3; i < 6; ++i) {
            if (board[i][8] == "BLACK") {
                ++count;
            }
        }
        if (board[1][4] == "BLACK") {
            ++count;
        }
        if (board[7][4] == "BLACK") {
            ++count;
        }
        if (board[4][1] == "BLACK") {
            ++count;
        }
        if (board[4][7] == "BLACK") {
            ++count;
        }
        return count;
    } catch (const std::exception& e) {
        cout << "An error occurred in count_black_starting_position: " << e.what() << endl;
        // logger.error("An error occurred in count_black_starting_position: " + std::string(e.what()));
        throw;
    }
}

std::pair<int, int> get_king_position(const std::vector<std::vector<std::string>>& board) {
    try {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (board[i][j] == "KING") {
                    return {i, j};
                }
            }
        }
        throw std::runtime_error("King not found on the board");
    } catch (const std::exception& e) {
        cout << "An error occurred in get_king_position: " << e.what() << endl;
        // logger.error("An error occurred in get_king_position: " + std::string(e.what()));
        throw;
    }
}

int king_winning_direction_score(const std::vector<std::vector<std::string>>& board) {
    try {
        auto king_position = get_king_position(board);
        return king_winning_direction_heatmap[king_position.first][king_position.second];
    } catch (const std::exception& e) {
        cout << "An error occurred in king_winning_direction_score: " << e.what() << endl;
        // logger.error("An error occurred in king_winning_direction_score: " + std::string(e.what()));
        throw;
    }
}

std::tuple<int, int, int, int, int> king_free_sides(const std::vector<std::vector<std::string>>& board) {
    try {
        auto king_position = get_king_position(board);

        int free_sides = 0;
        int blocked_sides = 0;
        int black_blockers = 0;
        int white_blockers = 0;
        int castle_blockers = 0;

        if (king_position.first == 0 || king_position.first == 8 || king_position.second == 0 || king_position.second == 8) {
            return {0, 0, 0, 0, 0};
        }

        auto check_side = [&](int row, int col) {
            if (board[row][col] == "EMPTY") {
                ++free_sides;
            } else {
                ++blocked_sides;
                if (std::find(black_camps_positions.begin(), black_camps_positions.end(), std::make_pair(row, col)) != black_camps_positions.end() || board[row][col] == "BLACK") {
                    ++black_blockers;
                } else if (board[row][col] == "WHITE") {
                    ++white_blockers;
                } else if (std::make_pair(row, col) == castle_position) {
                    ++castle_blockers;
                }
            }
        };

        check_side(king_position.first - 1, king_position.second);
        check_side(king_position.first + 1, king_position.second);
        check_side(king_position.first, king_position.second - 1);
        check_side(king_position.first, king_position.second + 1);

        return {free_sides, blocked_sides, black_blockers, white_blockers, castle_blockers};
    } catch (const std::exception& e) {
        cout << "An error occurred in king_free_sides: " << e.what() << endl;
        // logger.error("An error occurred in king_free_sides: " + std::string(e.what()));
        throw;
    }
}

bool black_checkmate(const std::vector<std::vector<std::string>>& board) {
    try {
        auto king_position = get_king_position(board);
        if (king_position.first == 0 || king_position.first == 8 || king_position.second == 0 || king_position.second == 8) {
            return false;
        }
        std::vector<std::pair<int, int>> move_directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        for (const auto& direction : move_directions) {
            auto new_position = std::make_pair(king_position.first + direction.first, king_position.second + direction.second);
            while (new_position.first >= 0 && new_position.first <= 8 && new_position.second >= 0 && new_position.second <= 8) {
                if (std::find(winning_positions.begin(), winning_positions.end(), new_position) != winning_positions.end()) {
                    return true;
                }
                if (board[new_position.first][new_position.second] != "EMPTY" || std::find(black_camps_positions.begin(), black_camps_positions.end(), new_position) != black_camps_positions.end() || new_position == castle_position) {
                    break;
                }
                new_position = std::make_pair(new_position.first + direction.first, new_position.second + direction.second);
            }
        }
        return false;
    } catch (const std::exception& e) {
        cout << "An error occurred in black_checkmate: " << e.what() << endl;
        // logger.error("An error occurred in black_checkmate: " + std::string(e.what()));
        throw;
    }
}

bool is_empty_and_reachable(const std::vector<std::vector<std::string>>& board, const std::pair<int, int>& position, const std::string& color) {
    try {
        if (board[position.first][position.second] != "EMPTY") {
            return false;
        }

        std::vector<std::pair<int, int>> move_directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

        // Check for the special rule
        if (std::find(black_camps_positions.begin(), black_camps_positions.end(), position) != black_camps_positions.end()) {
            for (const auto& direction : move_directions) {
                auto new_position = std::make_pair(position.first + direction.first, position.second + direction.second);
                while (new_position.first >= 0 && new_position.first <= 8 && new_position.second >= 0 && new_position.second <= 8) {
                    if (std::find(black_camps_positions.begin(), black_camps_positions.end(), new_position) != black_camps_positions.end()) {
                        if (board[new_position.first][new_position.second] == color) {
                            return true;
                        }
                        new_position = std::make_pair(new_position.first + direction.first, new_position.second + direction.second);
                    } else {
                        break;
                    }
                    if (board[new_position.first][new_position.second] != "EMPTY") {
                        break;
                    }
                }
            }
        }

        for (const auto& direction : move_directions) {
            auto new_position = std::make_pair(position.first + direction.first, position.second + direction.second);
            while (new_position.first >= 0 && new_position.first <= 8 && new_position.second >= 0 && new_position.second <= 8) {
                if (board[new_position.first][new_position.second] == color) { // if the position is a piece of the color
                    return true;
                }
                if (board[new_position.first][new_position.second] != "EMPTY") { // if the position is not empty
                    break;
                }
                if (std::find(black_camps_positions.begin(), black_camps_positions.end(), new_position) != black_camps_positions.end() || new_position == castle_position) { // if the position is a black camp or the castle
                    break;
                }
                new_position = std::make_pair(new_position.first + direction.first, new_position.second + direction.second);
            }
        }
        return false;
    } catch (const std::exception& e) {
        cout << "An error occurred in is_empty_and_reachable: " << e.what() << endl;
        // logger.error("An error occurred in is_empty_and_reachable: " + std::string(e.what()));
        throw;
    }
}

bool king_in_the_castle(const std::pair<int, int>& king_position) {
    try {
        return king_position == castle_position;
    } catch (const std::exception& e) {
        cout << "An error occurred in king_in_the_castle: " << e.what() << endl;
        // logger.error("An error occurred in king_in_the_castle: " + std::string(e.what()));
        throw;
    }
}

bool king_adjacent_to_castle(const std::pair<int, int>& king_position) {
    try {
        return king_position == std::make_pair(3, 4) || king_position == std::make_pair(5, 4) ||
               king_position == std::make_pair(4, 3) || king_position == std::make_pair(4, 5);
    } catch (const std::exception& e) {
        cout << "An error occurred in king_adjacent_to_castle: " << e.what() << endl;
        // logger.error("An error occurred in king_adjacent_to_castle: " + std::string(e.what()));
        throw;
    }
}

bool king_can_be_captured_between_two_blacks(const std::vector<std::vector<std::string>>& board) {
    try {
        auto king_position = get_king_position(board);
        if (king_position.first == 0 || king_position.first == 8 || king_position.second == 0 || king_position.second == 8) {
            return false;
        }
        if (king_in_the_castle(king_position) || king_adjacent_to_castle(king_position)) {
            return false;
        }
        if ((board[king_position.first - 1][king_position.second] == "BLACK" || std::find(black_camps_positions.begin(), black_camps_positions.end(), std::make_pair(king_position.first - 1, king_position.second)) != black_camps_positions.end()) &&
            is_empty_and_reachable(board, std::make_pair(king_position.first + 1, king_position.second), "BLACK")) {
            return true;
        }
        if ((board[king_position.first + 1][king_position.second] == "BLACK" || std::find(black_camps_positions.begin(), black_camps_positions.end(), std::make_pair(king_position.first + 1, king_position.second)) != black_camps_positions.end()) &&
            is_empty_and_reachable(board, std::make_pair(king_position.first - 1, king_position.second), "BLACK")) {
            return true;
        }
        if ((board[king_position.first][king_position.second - 1] == "BLACK" || std::find(black_camps_positions.begin(), black_camps_positions.end(), std::make_pair(king_position.first, king_position.second - 1)) != black_camps_positions.end()) &&
            is_empty_and_reachable(board, std::make_pair(king_position.first, king_position.second + 1), "BLACK")) {
            return true;
        }
        if ((board[king_position.first][king_position.second + 1] == "BLACK" || std::find(black_camps_positions.begin(), black_camps_positions.end(), std::make_pair(king_position.first, king_position.second + 1)) != black_camps_positions.end()) &&
            is_empty_and_reachable(board, std::make_pair(king_position.first, king_position.second - 1), "BLACK")) {
            return true;
        }
        return false;
    } catch (const std::exception& e) {
        cout << "An error occurred in king_can_be_captured_between_two_blacks: " << e.what() <<endl;
        // logger.error("An error occurred in king_can_be_captured_between_two_blacks: " + std::string(e.what()));
        throw;
    }
}

bool king_adjacent_to_castle_can_be_captured(const std::vector<std::vector<std::string>>& board) {
    try {
        auto king_position = get_king_position(board);
        if (king_position.first == 0 || king_position.first == 8 || king_position.second == 0 || king_position.second == 8) {
            return false;
        }
        if (!king_adjacent_to_castle(king_position)) {
            return false;
        }
        if (king_position == std::make_pair(3, 4)) {
            if ((board[3][3] == "BLACK" || std::find(black_camps_positions.begin(), black_camps_positions.end(), std::make_pair(3, 3)) != black_camps_positions.end()) &&
                is_empty_and_reachable(board, std::make_pair(3, 5), "BLACK") &&
                is_empty_and_reachable(board, std::make_pair(4, 4), "BLACK")) {
                return true;
            }
            if ((board[4][4] == "BLACK" || std::find(black_camps_positions.begin(), black_camps_positions.end(), std::make_pair(4, 4)) != black_camps_positions.end()) &&
                is_empty_and_reachable(board, std::make_pair(3, 3), "BLACK") &&
                is_empty_and_reachable(board, std::make_pair(3, 5), "BLACK")) {
                return true;
            }
            if ((board[3][5] == "BLACK" || std::find(black_camps_positions.begin(), black_camps_positions.end(), std::make_pair(3, 5)) != black_camps_positions.end()) &&
                is_empty_and_reachable(board, std::make_pair(3, 3), "BLACK") &&
                is_empty_and_reachable(board, std::make_pair(4, 4), "BLACK")) {
                return true;
            }
        }
        if (king_position == std::make_pair(5, 4)) {
            if ((board[5][3] == "BLACK" || std::find(black_camps_positions.begin(), black_camps_positions.end(), std::make_pair(5, 3)) != black_camps_positions.end()) &&
                is_empty_and_reachable(board, std::make_pair(5, 5), "BLACK") &&
                is_empty_and_reachable(board, std::make_pair(4, 4), "BLACK")) {
                return true;
            }
            if ((board[4][4] == "BLACK" || std::find(black_camps_positions.begin(), black_camps_positions.end(), std::make_pair(4, 4)) != black_camps_positions.end()) &&
                is_empty_and_reachable(board, std::make_pair(5, 3), "BLACK") &&
                is_empty_and_reachable(board, std::make_pair(5, 5), "BLACK")) {
                return true;
            }
            if ((board[5][5] == "BLACK" || std::find(black_camps_positions.begin(), black_camps_positions.end(), std::make_pair(5, 5)) != black_camps_positions.end()) &&
                is_empty_and_reachable(board, std::make_pair(5, 3), "BLACK") &&
                is_empty_and_reachable(board, std::make_pair(4, 4), "BLACK")) {
                return true;
            }
        }
        if (king_position == std::make_pair(4, 3)) {
            if ((board[3][3] == "BLACK" || std::find(black_camps_positions.begin(), black_camps_positions.end(), std::make_pair(3, 3)) != black_camps_positions.end()) &&
                is_empty_and_reachable(board, std::make_pair(5, 3), "BLACK") &&
                is_empty_and_reachable(board, std::make_pair(4, 4), "BLACK")) {
                return true;
            }
            if ((board[4][4] == "BLACK" || std::find(black_camps_positions.begin(), black_camps_positions.end(), std::make_pair(4, 4)) != black_camps_positions.end()) &&
                is_empty_and_reachable(board, std::make_pair(3, 3), "BLACK") &&
                is_empty_and_reachable(board, std::make_pair(5, 3), "BLACK")) {
                return true;
            }
            if ((board[5][3] == "BLACK" || std::find(black_camps_positions.begin(), black_camps_positions.end(), std::make_pair(5, 3)) != black_camps_positions.end()) &&
                is_empty_and_reachable(board, std::make_pair(3, 3), "BLACK") &&
                is_empty_and_reachable(board, std::make_pair(4, 4), "BLACK")) {
                return true;
            }
        }
        if (king_position == std::make_pair(4, 5)) {
            if ((board[3][5] == "BLACK" || std::find(black_camps_positions.begin(), black_camps_positions.end(), std::make_pair(3, 5)) != black_camps_positions.end()) &&
                is_empty_and_reachable(board, std::make_pair(5, 5), "BLACK") &&
                is_empty_and_reachable(board, std::make_pair(4, 4), "BLACK")) {
                return true;
            }
            if ((board[4][4] == "BLACK" || std::find(black_camps_positions.begin(), black_camps_positions.end(), std::make_pair(4, 4)) != black_camps_positions.end()) &&
                is_empty_and_reachable(board, std::make_pair(3, 5), "BLACK") &&
                is_empty_and_reachable(board, std::make_pair(5, 5), "BLACK")) {
                return true;
            }
            if ((board[5][5] == "BLACK" || std::find(black_camps_positions.begin(), black_camps_positions.end(), std::make_pair(5, 5)) != black_camps_positions.end()) &&
                is_empty_and_reachable(board, std::make_pair(3, 5), "BLACK") &&
                is_empty_and_reachable(board, std::make_pair(4, 4), "BLACK")) {
                return true;
            }
        }
        return false;
    } catch (const std::exception& e) {
        cout << "An error occurred in king_adjacent_to_castle_can_be_captured: " << e.what() << endl;
        // logger.error("An error occurred in king_adjacent_to_castle_can_be_captured: " + std::string(e.what()));
        throw;
    }
}

bool king_in_the_castle_can_be_captured(const std::vector<std::vector<std::string>>& board) {
    try {
        auto king_position = get_king_position(board);
        if (king_position.first == 0 || king_position.first == 8 || king_position.second == 0 || king_position.second == 8) {
            return false;
        }
        if (!king_in_the_castle(king_position)) {
            return false;
        }
        if (board[3][4] == "BLACK" && board[4][3] == "BLACK" && board[4][5] == "BLACK" && is_empty_and_reachable(board, std::make_pair(5, 4), "BLACK")) {
            return true;
        }
        if (board[5][4] == "BLACK" && board[4][3] == "BLACK" && board[4][5] == "BLACK" && is_empty_and_reachable(board, std::make_pair(3, 4), "BLACK")) {
            return true;
        }
        if (board[4][3] == "BLACK" && board[3][4] == "BLACK" && board[5][4] == "BLACK" && is_empty_and_reachable(board, std::make_pair(4, 5), "BLACK")) {
            return true;
        }
        if (board[4][5] == "BLACK" && board[3][4] == "BLACK" && board[5][4] == "BLACK" && is_empty_and_reachable(board, std::make_pair(4, 3), "BLACK")) {
            return true;
        }
        return false;
    } catch (const std::exception& e) {
        cout << "An error occurred in king_in_the_castle_can_be_captured: " << e.what() << endl;
        // logger.error("An error occurred in king_in_the_castle_can_be_captured: " + std::string(e.what()));
        throw;
    }
}

bool white_checkmate(const std::vector<std::vector<std::string>>& board) {
    try {
        return king_can_be_captured_between_two_blacks(board) ||
               king_adjacent_to_castle_can_be_captured(board) ||
               king_in_the_castle_can_be_captured(board);
    } catch (const std::exception& e) {
        cout << "An error occurred in white_checkmate: " << e.what() << endl;
        // logger.error("An error occurred in white_checkmate: " + std::string(e.what()));
        throw;
    }
}

bool king_can_be_captured_between_two_blacks_infuture(const std::vector<std::vector<std::string>>& board) {
    try {
        auto king_position = get_king_position(board);

        if (king_position.first == 0 || king_position.first == 8 || king_position.second == 0 || king_position.second == 8) {
            return false;
        }

        if (king_in_the_castle(king_position) || king_adjacent_to_castle(king_position)) {
            return false;
        }

        auto [reachable, starting_position] = is_empty_and_reachable_version2(board, {king_position.first - 1, king_position.second}, "BLACK");
        if (reachable && is_empty_and_reachable_version2(board, {king_position.first + 1, king_position.second}, "BLACK", starting_position).first) {
            return true;
        }

        std::tie(reachable, starting_position) = is_empty_and_reachable_version2(board, {king_position.first + 1, king_position.second}, "BLACK");
        if (reachable && is_empty_and_reachable_version2(board, {king_position.first - 1, king_position.second}, "BLACK", starting_position).first) {
            return true;
        }

        std::tie(reachable, starting_position) = is_empty_and_reachable_version2(board, {king_position.first, king_position.second - 1}, "BLACK");
        if (reachable && is_empty_and_reachable_version2(board, {king_position.first, king_position.second + 1}, "BLACK", starting_position).first) {
            return true;
        }

        std::tie(reachable, starting_position) = is_empty_and_reachable_version2(board, {king_position.first, king_position.second + 1}, "BLACK");
        if (reachable && is_empty_and_reachable_version2(board, {king_position.first, king_position.second - 1}, "BLACK", starting_position).first) {
            return true;
        }

        return false;
    } catch (const std::exception& e) {
        cout << "An error occurred in king_can_be_captured_between_two_blacks_infuture: " << e.what() << endl;
        // logger.error("An error occurred in king_can_be_captured_between_two_blacks_infuture: " + std::string(e.what()));
        throw;
    }
}

bool king_in_the_castle_can_be_captured_infuture(const std::vector<std::vector<std::string>>& board) {
    try {
        auto king_position = get_king_position(board);
        if (!king_in_the_castle(king_position)) {
            return false;
        }

        // Check if there are two black pieces on two sides and two more that can fill the remaining gaps
        auto check_positions = [&](const std::pair<int, int>& pos1, const std::pair<int, int>& pos2, const std::pair<int, int>& check1, const std::pair<int, int>& check2) {
            if (board[pos1.first][pos1.second] == "BLACK" && board[pos2.first][pos2.second] == "BLACK") {
                auto [reachable1, start_pos1] = is_empty_and_reachable_version2(board, check1, "BLACK");
                auto [reachable2, _] = is_empty_and_reachable_version2(board, check2, "BLACK", start_pos1);
                return reachable1 && reachable2;
            }
            return false;
        };

        if (check_positions({3, 4}, {4, 3}, {4, 5}, {5, 4})) return true;
        if (check_positions({3, 4}, {4, 5}, {4, 3}, {5, 4})) return true;
        if (check_positions({4, 3}, {5, 4}, {4, 5}, {3, 4})) return true;
        if (check_positions({4, 5}, {5, 4}, {4, 3}, {3, 4})) return true;
        if (check_positions({3, 4}, {5, 4}, {4, 3}, {4, 5})) return true;
        if (check_positions({4, 3}, {4, 5}, {3, 4}, {5, 4})) return true;

        return false;
    } catch (const std::exception& e) {
        cout << "An error occurred in king_in_the_castle_can_be_captured_infuture: " << e.what() << endl;
        // logger.error("An error occurred in king_in_the_castle_can_be_captured_infuture: " + std::string(e.what()));
        throw;
    }
}

bool king_adjacent_to_castle_can_be_captured_infuture(const std::vector<std::vector<std::string>>& board) {
    try {
        auto king_position = get_king_position(board);
        if (!king_adjacent_to_castle(king_position)) {
            return false;
        }

        // Check if there is one black piece and two more that can fill the remaining gaps
        auto check_positions = [&](const std::pair<int, int>& pos1, const std::pair<int, int>& pos2, const std::pair<int, int>& check1, const std::pair<int, int>& check2) {
            if (board[pos1.first][pos1.second] == "BLACK" || std::find(black_camps_positions.begin(), black_camps_positions.end(), pos1) != black_camps_positions.end()) {
                auto [reachable1, start_pos1] = is_empty_and_reachable_version2(board, check1, "BLACK");
                auto [reachable2, _] = is_empty_and_reachable_version2(board, check2, "BLACK", start_pos1);
                return reachable1 && reachable2;
            }
            return false;
        };

        if (king_position == std::make_pair(3, 4)) {
            if (check_positions({3, 3}, {3, 5}, {4, 4}, {3, 5})) return true;
            if (check_positions({3, 5}, {3, 3}, {4, 4}, {3, 3})) return true;
            if (check_positions({4, 4}, {3, 3}, {3, 5}, {3, 3})) return true;
        }
        if (king_position == std::make_pair(5, 4)) {
            if (check_positions({5, 3}, {5, 5}, {4, 4}, {5, 5})) return true;
            if (check_positions({5, 5}, {5, 3}, {4, 4}, {5, 3})) return true;
            if (check_positions({4, 4}, {5, 3}, {5, 5}, {5, 3})) return true;
        }
        if (king_position == std::make_pair(4, 3)) {
            if (check_positions({3, 3}, {5, 3}, {4, 4}, {5, 3})) return true;
            if (check_positions({5, 3}, {3, 3}, {4, 4}, {3, 3})) return true;
            if (check_positions({4, 4}, {3, 3}, {5, 3}, {3, 3})) return true;
        }
        if (king_position == std::make_pair(4, 5)) {
            if (check_positions({3, 5}, {5, 5}, {4, 4}, {5, 5})) return true;
            if (check_positions({5, 5}, {3, 5}, {4, 4}, {3, 5})) return true;
            if (check_positions({4, 4}, {3, 5}, {5, 5}, {3, 5})) return true;
        }

        return false;
    } catch (const std::exception& e) {
        cout << "An error occurred in king_adjacent_to_castle_can_be_captured_infuture: " << e.what() << endl;
        // logger.error("An error occurred in king_adjacent_to_castle_can_be_captured_infuture: " + std::string(e.what()));
        throw;
    }
}

bool king_can_checkmate_in_future(const std::vector<std::vector<std::string>>& board) {
    try {
        auto king_position = get_king_position(board);
        std::vector<std::pair<int, int>> move_directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

        // Check if the king can win in two moves
        for (const auto& direction : move_directions) {
            auto first_move = std::make_pair(king_position.first + direction.first, king_position.second + direction.second);
            while (first_move.first >= 0 && first_move.first <= 8 && first_move.second >= 0 && first_move.second <= 8) {
                if (board[first_move.first][first_move.second] == "EMPTY") {
                    for (const auto& second_direction : move_directions) {
                        auto second_move = std::make_pair(first_move.first + second_direction.first, first_move.second + second_direction.second);
                        while (second_move.first >= 0 && second_move.first <= 8 && second_move.second >= 0 && second_move.second <= 8) {
                            if (std::find(winning_positions.begin(), winning_positions.end(), second_move) != winning_positions.end()) {
                                return true;
                            }
                            if (board[second_move.first][second_move.second] != "EMPTY" || std::find(black_camps_positions.begin(), black_camps_positions.end(), second_move) != black_camps_positions.end() || second_move == castle_position) {
                                break;
                            }
                            second_move = std::make_pair(second_move.first + second_direction.first, second_move.second + second_direction.second);
                        }
                    }
                }
                if (board[first_move.first][first_move.second] != "EMPTY" || std::find(black_camps_positions.begin(), black_camps_positions.end(), first_move) != black_camps_positions.end() || first_move == castle_position) {
                    break;
                }
                first_move = std::make_pair(first_move.first + direction.first, first_move.second + direction.second);
            }
        }

        return false;
    } catch (const std::exception& e) {
        cout << "An error occurred in king_can_checkmate_in_future: " << e.what() << endl;
        // logger.error("An error occurred in king_can_checkmate_in_future: " + std::string(e.what()));
        throw;
    }
}

bool king_block_in_quadrant(const std::vector<std::vector<std::string>>& board) {
    try {
        auto [row, col] = get_king_position(board);

        // If the king is on a corridor then is not blocked
        if (row == 4 || col == 4) {
            return false;
        }

        // Based on the quadrant i get the areas to check
        std::vector<std::pair<int, int>> orizontal;
        std::vector<std::pair<int, int>> vertical;
        bool blocked_orizontal = false;
        bool blocked_vertical = false;

        if (row < 4) {
            orizontal = {{2, 3}, {2, 4}, {2, 5}, {3, 3}, {3, 4}, {3, 5}};
        } else {
            orizontal = {{5, 3}, {5, 4}, {5, 5}, {6, 3}, {6, 4}, {6, 5}};
        }

        if (col < 4) {
            vertical = {{3, 2}, {3, 3}, {4, 2}, {4, 3}, {5, 2}, {5, 3}};
        } else {
            vertical = {{3, 5}, {3, 6}, {4, 5}, {4, 6}, {5, 5}, {5, 6}};
        }

        // If the king is in one of the area then i delete that position from the list
        auto it = std::find(orizontal.begin(), orizontal.end(), std::make_pair(row, col));
        if (it != orizontal.end()) {
            orizontal.erase(it);
        }

        it = std::find(vertical.begin(), vertical.end(), std::make_pair(row, col));
        if (it != vertical.end()) {
            vertical.erase(it);
        }

        // For the two areas i check if are they blocked
        for (const auto& t1 : orizontal) {
            for (const auto& t2 : orizontal) {
                if (board[t1.first][t1.second] == "BLACK" && board[t2.first][t2.second] == "BLACK" && t1.first != t2.first && std::abs(t1.second - t2.second) < 2) {
                    blocked_orizontal = true;
                }
            }
        }

        for (const auto& t1 : vertical) {
            for (const auto& t2 : vertical) {
                if (board[t1.first][t1.second] == "BLACK" && board[t2.first][t2.second] == "BLACK" && t1.second != t2.second && std::abs(t1.first - t2.first) < 2) {
                    blocked_vertical = true;
                }
            }
        }

        return blocked_orizontal && blocked_vertical;
    } catch (const std::exception& e) {
        cout << "An error occurred in king_block_in_quadrant: " << e.what() << endl;
        // logger.error("An error occurred in king_block_in_quadrant: " + std::string(e.what()));
        throw;
    }
}

bool black_can_checkmate_in_future(const std::vector<std::vector<std::string>>& board) {
    try {
        return king_can_be_captured_between_two_blacks_infuture(board) ||
               king_adjacent_to_castle_can_be_captured_infuture(board) ||
               king_in_the_castle_can_be_captured_infuture(board);
    } catch (const std::exception& e) {
        cout << "An error occurred in black_can_checkmate_in_future: " << e.what() << endl;
        // logger.error("An error occurred in black_can_checkmate_in_future: " + std::string(e.what()));
        throw;
    }
}

std::pair<bool, std::pair<int, int>> is_empty_and_reachable_version2(const std::vector<std::vector<std::string>>& board, const std::pair<int, int>& position, const std::string& color, const std::pair<int, int>& exceptional_start = {-1, -1}) {
    try {
        if (board[position.first][position.second] != "EMPTY") {
            return {false, {-1, -1}};
        }

        std::vector<std::pair<int, int>> move_directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

        // Check for the special rule
        if (std::find(black_camps_positions.begin(), black_camps_positions.end(), position) != black_camps_positions.end()) {
            for (const auto& direction : move_directions) {
                auto new_position = std::make_pair(position.first + direction.first, position.second + direction.second);
                while (new_position.first >= 0 && new_position.first <= 8 && new_position.second >= 0 && new_position.second <= 8) {
                    if (std::find(black_camps_positions.begin(), black_camps_positions.end(), new_position) != black_camps_positions.end()) {
                        if (board[new_position.first][new_position.second] == color && new_position != exceptional_start) {
                            return {true, new_position};
                        }
                        new_position = std::make_pair(new_position.first + direction.first, new_position.second + direction.second);
                    } else {
                        break;
                    }
                    if (board[new_position.first][new_position.second] != "EMPTY") {
                        break;
                    }
                }
            }
        }

        for (const auto& direction : move_directions) {
            auto new_position = std::make_pair(position.first + direction.first, position.second + direction.second);
            while (new_position.first >= 0 && new_position.first <= 8 && new_position.second >= 0 && new_position.second <= 8) {
                if (board[new_position.first][new_position.second] == color && new_position != exceptional_start) {  // if the position is a piece of the color and not the exceptional start
                    return {true, new_position};
                }
                if (board[new_position.first][new_position.second] != "EMPTY") {  // if the position is not empty
                    break;
                }
                if (std::find(black_camps_positions.begin(), black_camps_positions.end(), new_position) != black_camps_positions.end() || new_position == castle_position) {  // if the position is a black camp or the castle
                    break;
                }
                new_position = std::make_pair(new_position.first + direction.first, new_position.second + direction.second);
            }
        }
        return {false, {-1, -1}};
    } catch (const std::exception& e) {
        cout << "An error occurred in is_empty_and_reachable_version2: " << e.what() << endl; 
        // logger.error("An error occurred in is_empty_and_reachable_version2: " + std::string(e.what()));
        throw;
    }
}

std::pair<bool, bool> situation_king_sides(const std::vector<std::vector<std::string>>& board) {
    try {
        auto [free_sides, blocked_sides, black_blockers, white_blockers, castle_blockers] = king_free_sides(board);
        auto king_position = get_king_position(board);

        if (king_position.first == 0 || king_position.first == 8 || king_position.second == 0 || king_position.second == 8) {
            return {false, false};
        }

        bool white_opposite = false;
        bool black_opposite = false;

        if (white_blockers == 2) {
            if (board[king_position.first - 1][king_position.second] == "WHITE" && board[king_position.first + 1][king_position.second] == "WHITE") {
                white_opposite = true;
            }
            if (board[king_position.first][king_position.second - 1] == "WHITE" && board[king_position.first][king_position.second + 1] == "WHITE") {
                white_opposite = true;
            }
        }

        if (black_blockers == 2) {
            if ((board[king_position.first - 1][king_position.second] == "BLACK" || std::find(black_camps_positions.begin(), black_camps_positions.end(), std::make_pair(king_position.first - 1, king_position.second)) != black_camps_positions.end()) &&
                (board[king_position.first + 1][king_position.second] == "BLACK" || std::find(black_camps_positions.begin(), black_camps_positions.end(), std::make_pair(king_position.first + 1, king_position.second)) != black_camps_positions.end())) {
                black_opposite = true;
            }
            if ((board[king_position.first][king_position.second - 1] == "BLACK" || std::find(black_camps_positions.begin(), black_camps_positions.end(), std::make_pair(king_position.first, king_position.second - 1)) != black_camps_positions.end()) &&
                (board[king_position.first][king_position.second + 1] == "BLACK" || std::find(black_camps_positions.begin(), black_camps_positions.end(), std::make_pair(king_position.first, king_position.second + 1)) != black_camps_positions.end())) {
                black_opposite = true;
            }
        }

        return {white_opposite, black_opposite};
    } catch (const std::exception& e) {
        cout << "An error occurred in situation_king_sides: " << e.what() << endl;
        // logger.error("An error occurred in situation_king_sides: " + std::string(e.what()));
        throw;
    }
}

std::string stats_of_the_board(const std::vector<std::vector<std::string>>& board, const std::string& turn) {
    std::ostringstream result;

    result << "\n----------NUMBER OF PIECES-----------\n";
    result << "White pieces: " << count_pieces(board, "WHITE") << "/8 + the KING\n";
    result << "Black pieces: " << count_pieces(board, "BLACK") << "/16\n";
    result << "\n-------------------------------------\n";

    result << "-----PIECES ON STARTING POSITION-----\n";
    result << "White pieces on starting position: " << count_white_starting_position(board) << "/8\n";
    result << "Black pieces on starting position: " << count_black_starting_position(board) << "/16\n";
    result << "\n-------------------------------------\n";

    result << "-----------KING SITUATION------------\n";
    result << "The KING is in a position with a direction winning score of: " << king_winning_direction_score(board) << "\n";
    result << "The KING is in the castle: " << (king_in_the_castle(get_king_position(board)) ? "Yes" : "No") << "\n";
    result << "The KING is adjacent to the castle: " << (king_adjacent_to_castle(get_king_position(board)) ? "Yes" : "No") << "\n";
    auto [free_sides, blocked_sides, black_blockers, white_blockers, castle_blockers] = king_free_sides(board);
    result << "The KING has free sides: " << free_sides << "/4\n";
    result << "The KING has blocked sides: " << blocked_sides << "/4\n";
    result << "The KING has black blockers: " << black_blockers << "/" << blocked_sides << "\n";
    result << "The KING has white blockers: " << white_blockers << "/" << blocked_sides << "\n";
    result << "The KING has castle blockers: " << castle_blockers << "/" << blocked_sides << "\n";
    result << "The KING is blocked in a quadrant: " << (king_block_in_quadrant(board) ? "Yes" : "No") << "\n";
    result << "\n-------------------------------------\n";

    if (turn == "black") {
        result << "---------BLACK CHECKMATE-------------\n";
        result << "The BLACK is in checkmate: " << (black_checkmate(board) ? "Yes" : "No") << "\n";
        result << "\n-------------------------------------\n";
    }

    if (turn == "white") {
        result << "---------WHITE CHECKMATE-------------\n";
        result << "The WHITE is in checkmate: " << (white_checkmate(board) ? "Yes" : "No") << "\n";
        result << "\n-------------------------------------\n";
    }

    return result.str();
}

std::string format_board(const std::vector<std::vector<std::string>>& board) {
    std::unordered_map<std::string, char> symbols = {
        {"WHITE", 'W'},
        {"BLACK", 'B'},
        {"KING", 'K'},
        {"THRONE", 'T'},
        {"EMPTY", ' '}
    };

    std::string column_labels = "A B C D E F G H I";
    std::ostringstream board_str;
    board_str << "    " << column_labels << "\n";
    board_str << "  +" << std::string(9 * 4 - 1, '-') << "+\n";

    for (int i = 0; i < 9; ++i) {
        board_str << i + 1 << " | ";
        for (int j = 0; j < 9; ++j) {
            board_str << symbols[board[i][j]] << " | ";
        }
        board_str << "\n";
        board_str << "  +" << std::string(9 * 4 - 1, '-') << "+\n";
    }

    return board_str.str();
}