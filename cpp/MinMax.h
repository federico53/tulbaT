#ifndef MINMAX_H
#define MINMAX_H

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include "GameUtils.h"


using namespace std;

// EVALUAZIONE EURISTICA
int heuristic_white(const vector<vector<int>>& board);
int calculate_black_starting_positions_points(const vector<vector<string>>& board);
int heuristic_black(const vector<vector<string>>& board);
int heuristic(const vector<vector<int>>& board, const string& turn);
int heuristic_evaluation(const vector<vector<int>>& board, const string& turn, const string& player);

// MINIMAX ALPHA BETA PRUNING
pair<int, Move> minimax_alpha_beta(vector<vector<int>>& board, int depth, int alpha, int beta, const string& turn, const string& player);


#endif 
