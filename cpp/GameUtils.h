#ifndef GAME_UTILS_H
#define GAME_UTILS_H

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include "MinMax.h"
#include "GameUtils.h"
#include "Stats.h"
#include "Logger.h"

using namespace std;

// COSTANTI E UTILITY
extern const vector<pair<int, int>> winning_positions;

struct Move {
    std::pair<int, int> from;
    std::pair<int, int> to;
};

// PARSE MOVE
Move parseMove(const std::string& move);

// FUNZIONI DI VALIDAZIONE POSIZIONI
bool is_within_bounds(int row, int col);
bool is_castle(int row, int col);
bool is_citadel(int row, int col);
bool is_adjacent_to_castle(int row, int col);
bool is_blocking_cell(int row, int col);
bool is_king_captured(const vector<vector<char>>& board);

// FUNZIONI PER IL TURNOVER
char get_opposite_turn(const char &turn);

// FUNZIONI DI VALIDAZIONE MOSSE
bool is_valid_move(Move &move, const vector<vector<char>> &board, const char &color);

// GENERAZIONE MOSSE
vector<Move> generate_all_possible_moves(const vector<vector<char>> &board, const char &color);

// GAME OVER
std::string is_game_over(const std::vector<std::vector<char>>& board);

// APPLICAZIONE MOSSE
std::vector<std::vector<char>> apply_move(const std::vector<std::vector<char>>& board, const Move& move);
bool is_enemy(const char& enemy, const char& of);
bool is_allie(const char& allie, const char& of);

// BOARD REPRESENTATION
std::string format_board(const std::vector<std::vector<char>>& board);

#endif 