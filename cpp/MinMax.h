#ifndef MINMAX_H
#define MINMAX_H

#include <vector>
#include <iostream>
#include <limits>
#include "GameUtils.h"
#include "Stats.h" 

using namespace std;

// EVALUAZIONE EURISTICA
int heuristic_white(const vector<vector<char>>& board);
int calculate_black_starting_positions_points(const vector<vector<char>>& board);
int heuristic_black(const vector<vector<char>>& board);
int heuristic(const vector<vector<char>>& board, const char& turn);
int heuristic_evaluation(const vector<vector<char>>& board, const char& turn, const char& player);

// MINIMAX ALPHA BETA PRUNING
pair<int, Move> minimax_alpha_beta(const vector<vector<char>>& board, int depth, int alpha, int beta, const char& turn, const char& player);


#endif 
