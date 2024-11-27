#ifndef MINMAX_H
#define MINMAX_H

#include <vector>
#include <iostream>
#include <limits>
#include "GameUtils.h"
#include "Stats.h" 
#include "Logger.h"

using namespace std;

// EVALUAZIONE EURISTICA
std::map<char, std::map<char, int>> get_stats(const vector<vector<char>>& board);
std::tuple<int, int> get_points(const vector<vector<char>>& board);
int heuristic_evaluation(const vector<vector<char>>& board, const char& player, const char& game_over);

// MINIMAX ALPHA BETA PRUNING
pair<int, Move> minimax_alpha_beta(const vector<vector<char>>& board, int depth, int alpha, int beta, const char& turn, const char& player);
pair<int, Move> minimax_alpha_beta_fast(const vector<vector<char>>& board, int depth, int alpha, int beta, const char& turn, const char& player, int cut_size);
pair<int, Move> run_minimax(const vector<vector<char>>& board, int depth, const char& turn, const char& player, int cut_size);
pair<int, Move> run_minimax_with_threads(const vector<vector<char>>& board, int depth, const char& turn, const char& player, int cut_size);
pair<int, Move> minimax_alpha_beta_fast_with_thread(const vector<vector<char>>& board, int depth, int alpha, int beta, const char& turn, const char& player, int cut_size);


#endif 
