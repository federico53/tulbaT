#ifndef STATS_H
#define STATS_H

#include <vector>
#include <sstream>
#include <unordered_map>
#include <iostream>
#include <algorithm>

#include "GameUtils.h"
#include "Logger.h"

using namespace std;

bool black_checkmate(const vector<vector<char>>& board, const pair<int, int>& king_position);

bool is_empty_and_reachable(const vector<vector<char>>& board, const pair<int, int>& position, const char color);

bool king_adjacent_to_castle(const pair<int, int>& king_position);

bool king_can_be_captured_between_two_blacks(const vector<vector<char>>& board, const pair<int, int>& king_position);
bool king_adjacent_to_castle_can_be_captured(const vector<vector<char>>& board, const pair<int, int>& king_position);
bool king_in_the_castle_can_be_captured(const vector<vector<char>>& board, const pair<int, int>& king_position);

bool white_checkmate(const vector<vector<char>>& board, const pair<int, int>& king_position);

std::string format_board(const vector<vector<char>>& board);

#endif 
