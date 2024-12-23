#ifndef GAME_UTILS_H
#define GAME_UTILS_H

#include <algorithm>
#include <map>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include "Stats.h"

using namespace std;

struct Move {
    std::pair<int, int> from;
    std::pair<int, int> to;
};

// PARSE MOVE
Move parseMove(const std::string& move);

// FUNZIONI DI VALIDAZIONE POSIZIONI
bool is_within_bounds(int row, int col);
bool is_citadel(int row, int col);
bool is_winning_position(int row, int col);
bool is_adjacent_to_castle(int row, int col);
bool is_blocking_cell(int row, int col);
bool is_king_captured(const vector<vector<char>>& board, const pair<int, int>& king_position);

// FUNZIONI PER IL TURNOVER
char get_opposite_turn(const char &turn);

// FUNZIONI DI VALIDAZIONE MOSSE
bool is_valid_move(Move &move, const vector<vector<char>> &board, const char &color);

// GENERAZIONE MOSSE
vector<Move> generate_all_possible_moves(const vector<vector<char>> &board, const char &color);

// GAME OVER
char is_game_over(const std::vector<std::vector<char>>& board);

// APPLICAZIONE MOSSE
bool is_enemy(const char& enemy, const char& of);
bool is_allie(const char& allie, const char& of, const std::vector<std::vector<char>>& board);
std::vector<std::vector<char>> apply_move(const std::vector<std::vector<char>>& board, const Move& move);

// BOARD REPRESENTATION
std::string format_board(const std::vector<std::vector<char>>& board);

#endif 
