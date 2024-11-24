#ifndef STATS_H
#define STATS_H

#include <vector>
#include <sstream>
#include <unordered_map>
#include <iostream>
#include <algorithm>

#include "GameUtils.h"

using namespace std;

// COSTANTI E UTILITY
extern const vector<vector<int>> king_winning_direction_heatmap;
extern const vector<pair<int, int>> black_camps_positions;
extern const pair<int, int> castle_position;
extern const std::vector<std::pair<int, int>> winning_positions;


// FUNZIONI DI STATISTICHE
int count_pieces(const vector<vector<char>>& board, char player);
int count_white_starting_position(const vector<vector<char>>& board);
int count_black_starting_position(const vector<vector<char>>& board);

pair<int, int> get_king_position(const vector<vector<char>>& board);

int king_winning_direction_score(const vector<vector<char>>& board);
tuple<int, int, int, int, int> king_free_sides(const vector<vector<char>>& board);

bool black_checkmate(const vector<vector<char>>& board);

bool is_empty_and_reachable(const vector<vector<char>>& board, const pair<int, int>& position, const char color);
std::pair<bool, std::pair<int, int>> is_empty_and_reachable_version2(const vector<vector<char>>& board, const std::pair<int, int>& position, const char color, const std::pair<int, int>& exceptional_start);

bool king_in_the_castle(const pair<int, int>& king_position);
bool king_adjacent_to_castle(const pair<int, int>& king_position);

bool king_can_be_captured_between_two_blacks(const vector<vector<char>>& board);
bool king_adjacent_to_castle_can_be_captured(const vector<vector<char>>& board);
bool king_in_the_castle_can_be_captured(const vector<vector<char>>& board);

bool white_checkmate(const vector<vector<char>>& board);

bool king_can_be_captured_between_two_blacks_infuture(const vector<vector<char>>& board);
bool king_in_the_castle_can_be_captured_infuture(const vector<vector<char>>& board);
bool king_adjacent_to_castle_can_be_captured_infuture(const vector<vector<char>>& board);
bool king_can_checkmate_in_future(const vector<vector<char>>& board);

bool king_block_in_quadrant(const vector<vector<char>>& board);

bool black_can_checkmate_in_future(const vector<vector<char>>& board);

std::pair<bool, bool> situation_king_sides(const vector<vector<char>>& board);

std::string stats_of_the_board(const vector<vector<char>>& board, const char& turn);
std::string format_board(const vector<vector<char>>& board);

#endif 
