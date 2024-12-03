
#include <iostream>
#include <memory>
#include "headers/Logger.h"
#include "headers/EnginePlayer.h"
#include "headers/HumanPlayer.h"
#include "headers/MinMax.h"
#include "headers/Stats.h"
#include "headers/GameUtils.h"

using namespace std;

#include <iostream>

int main() {
    // Dichiarazione e inizializzazione della matrice 9x9 con valori a piacere
    vector<vector<char>> board = {
        {'E', 'E', 'E', 'E', 'B', 'B', 'E', 'E', 'E'},
        {'E', 'E', 'B', 'E', 'E', 'E', 'E', 'E', 'E'},
        {'B', 'E', 'E', 'E', 'W', 'E', 'E', 'E', 'E'},
        {'E', 'E', 'E', 'E', 'W', 'E', 'B', 'E', 'E'},
        {'B', 'E', 'E', 'E', 'T', 'E', 'B', 'E', 'B'},
        {'E', 'E', 'E', 'E', 'W', 'E', 'B', 'E', 'E'},
        {'E', 'E', 'E', 'E', 'W', 'E', 'E', 'E', 'E'},
        {'B', 'E', 'K', 'E', 'B', 'E', 'E', 'E', 'E'},
        {'E', 'W', 'E', 'E', 'B', 'B', 'E', 'E', 'E'}
    };

    // Stampa della matrice
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            std::cout << board[i][j] << ' ';
        }
        std::cout << std::endl;
    }

    // MINMAX

    int score = 0;
    Move move;

    std::pair<int, Move> result = minimax_alpha_beta_fast(board, 3, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), 'B', 'B', 100);

    cout << "Best score: " << result.first << endl;
    cout << "Best move: " << result.second.from.first << " " << result.second.from.second << " to: " << result.second.to.first << " " << result.second.to.second << endl;

    auto stats = get_stats(board);

    // Stampa delle statistiche
    for (const auto& [key, value] : stats) {
        cout << key << ": ";
        for (const auto& [k, v] : value) {
            cout << k << " -> " << v << " ";
        }
        cout << endl;
    }

    cout << "Game over: " << is_game_over(board) << endl;

    // cout << double_escape_for_the_king(board) << endl;

    //cout << stats_of_the_board(board, 'B') << endl;
    //cout << "The white is in checkmate: " << (white_checkmate(board) ? "Yes" : "No") << endl;
    //cout << "Empty and reachable: " << (is_empty_and_reachable(board, {8, 0}, 'W') ? "Yes" : "No") << endl;

    // Move move;
    // move.from={4,1};
    // move.to={4,2};
    // vector<vector<char>> new_board = apply_move(board, move);
    // // Stampa della matrice
    // for (int i = 0; i < 9; ++i) {
    //     for (int j = 0; j < 9; ++j) {
    //         std::cout << new_board[i][j] << ' ';
    //     }
    //     std::cout << std::endl;
    // }

    return 0;
}
