#include <iostream>
#include <memory>
#include "Logger.h"
#include "EnginePlayer.h"
#include "HumanPlayer.h"

using namespace std;

#include <iostream>

int main() {
    // Dichiarazione e inizializzazione della matrice 9x9 con valori a piacere
    vector<vector<char>> board = {
        {'E', 'E', 'E', 'B', 'B', 'B', 'E', 'E', 'E'},
        {'E', 'E', 'E', 'E', 'E', 'E', 'B', 'B', 'E'},
        {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
        {'E', 'E', 'E', 'B', 'E', 'W', 'E', 'E', 'B'},
        {'B', 'B', 'E', 'K', 'T', 'B', 'E', 'E', 'B'},
        {'E', 'E', 'E', 'B', 'E', 'W', 'W', 'E', 'E'},
        {'E', 'E', 'E', 'E', 'W', 'E', 'E', 'E', 'E'},
        {'E', 'E', 'E', 'E', 'E', 'B', 'E', 'E', 'B'},
        {'E', 'E', 'E', 'E', 'B', 'E', 'E', 'E', 'E'}
    };

    // Stampa della matrice
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            std::cout << board[i][j] << ' ';
        }
        std::cout << std::endl;
    }

    cout << stats_of_the_board(board, 'B') << endl;
    cout << stats_of_the_board(board, 'W') << endl;

    Move move;
    move.from={3,8};
    move.to={3,6};
    std::vector<std::vector<char>> new_board = apply_move(board, move);

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            std::cout << new_board[i][j] << ' ';
        }
        std::cout << std::endl;
    }

    cout << "Game over:" <<is_game_over(board) << endl;
    cout << "King captured:" << (is_king_captured(board) ? "Yes" : "No") << endl;

    return 0;
}