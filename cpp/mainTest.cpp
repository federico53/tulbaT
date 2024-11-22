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
        {'E', 'B', 'E', 'B', 'E', 'B', 'E', 'E', 'E'},
        {'W', 'K', 'E', 'E', 'E', 'E', 'B', 'E', 'E'},
        {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
        {'E', 'E', 'W', 'B', 'E', 'B', 'E', 'E', 'B'},
        {'E', 'E', 'E', 'E', 'T', 'W', 'W', 'E', 'B'},
        {'E', 'B', 'E', 'E', 'W', 'E', 'E', 'E', 'B'},
        {'E', 'B', 'E', 'E', 'W', 'E', 'E', 'E', 'E'},
        {'E', 'B', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
        {'E', 'B', 'E', 'B', 'E', 'B', 'E', 'E', 'E'}
    };

    // Stampa della matrice
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            std::cout << board[i][j] << ' ';
        }
        std::cout << std::endl;
    }

    cout << stats_of_the_board(board, 'B') << endl;
    cout << "The white is in checkmate: " << (white_checkmate(board) ? "Yes" : "No") << endl;
    cout << "Empty and reachable: " << (is_empty_and_reachable(board, {8, 0}, 'W') ? "Yes" : "No") << endl;


    return 0;
}