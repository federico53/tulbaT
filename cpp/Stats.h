#ifndef STATS_H
#define STATS_H

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <unordered_map>

using namespace std;

// COSTANTI E UTILITY
extern const vector<vector<int>> king_winning_direction_heatmap
extern const vector<pair<int, int>> black_camps_positions;
extern const pair<int, int> castle_position;

// FUNZIONI DI STATISTICHE
int count_pieces(const vector<vector<string>>& board, const string& player);
int count_white_starting_position(const vector<vector<string>>& board);
int count_black_starting_position(const vector<vector<string>>& board);
pair<int, int> get_king_position(const vector<vector<string>>& board);
int king_winning_direction_score(const vector<vector<string>>& board);
tuple<int, int, int, int, int> king_free_sides(const vector<vector<string>>& board);
bool king_in_the_castle(const pair<int, int>& king_position);
bool king_adjacent_to_castle(const pair<int, int>& king_position);
bool king_block_in_quadrant(const vector<vector<string>>& board);
bool king_can_checkmate_in_future(const vector<vector<string>>& board);
bool king_adjacent_to_castle_can_be_captured_infuture(const vector<vector<string>>& board);
bool king_adjacent_to_castle_can_be_captured_infuture(const vector<vector<string>>& board);
bool king_can_checkmate_in_future(const vector<vector<string>>& board);
bool king_block_in_quadrant(const vector<vector<string>>& board);
bool black_checkmate(const vector<vector<string>>& board);
bool white_checkmate(const vector<vector<string>>& board);
bool is_game_over(const vector<vector<string>>& board);
string format_board(const vector<vector<string>>& board);

#endif 
