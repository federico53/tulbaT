#ifndef CHESS_UTILS_H
#define CHESS_UTILS_H

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <sstream>

using namespace std;

// COSTANTI E UTILITY
extern const vector<vector<string>> empty_board;
extern const vector<pair<int, int>> citadels;
extern const vector<pair<int, int>> winning_positions;

// FUNZIONI DI VALIDAZIONE POSIZIONI
bool is_within_bounds(int row, int col);
bool is_castle(int row, int col);
bool is_adjacent_to_castle(int row, int col);
bool is_king_captured(const vector<vector<string>>& board);
bool is_blocking_cell(int row, int col, const vector<vector<string>>& board);

// FUNZIONI PER IL TURNOVER
string get_opposite_turn(const string &turn);

// FUNZIONI DI VALIDAZIONE MOSSE
bool is_valid_move(pair<int, int> from, pair<int, int> to, const vector<vector<string>> &board, const string &color);

// GENERAZIONE MOSSE
vector<pair<pair<int, int>, pair<int, int>>> generate_all_possible_moves(const vector<vector<string>> &board, const string &color);

// EVALUAZIONE EURISTICA
int heuristic_white(const vector<vector<int>>& board);
int calculate_black_starting_positions_points(const vector<vector<string>>& board);
int heuristic_black(const vector<vector<string>>& board);
int heuristic_evaluation(const vector<vector<int>>& board, const string& turn, const string& player);
int heuristic(const vector<vector<int>>& board, const string& turn);

// MINIMAX ALPHA BETA PRUNING
pair<int, Move> minimax_alpha_beta(vector<vector<int>>& board, int depth, int alpha, int beta, const string& turn, const string& player);

// APPLICAZIONE MOSSE
std::vector<std::vector<std::string>> apply_move(const std::vector<std::vector<std::string>>& board, const std::pair<std::pair<int, int>, std::pair<int, int>>& move);

#endif // CHESS_UTILS_H
