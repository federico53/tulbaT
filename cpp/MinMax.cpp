#include "headers/MinMax.h"

using namespace std;

int COUNTER = 0;

std::map<char, std::map<char, int>> get_stats(const vector<vector<char>>& board) {
    try {
        map<char, map<char, int>> stats;
        vector<pair<int, int>> black_pieces;
        int white_pieces = 0;
        int free_sides = 0, blocked_sides = 0, black_blockers = 0, white_blockers = 0, castle_blockers = 0;
        float near_king = 0;
        static const vector<vector<int>> king_winning_direction_heatmap = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 2, 3, 1, 0, 1, 3, 2, 0},
            {0, 3, 4, 2, 2, 2, 4, 3, 0},
            {0, 1, 2, 0, 0, 0, 2, 1, 0},
            {0, 0, 2, 0, 0, 0, 2, 0, 0},
            {0, 1, 2, 0, 0, 0, 2, 1, 0},
            {0, 3, 4, 2, 2, 2, 4, 3, 0},
            {0, 2, 3, 1, 0, 1, 3, 2, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0}
        };

        for(int row = 0; row < 9; ++row){
            for(int col = 0; col < 9; ++col){
                
                // Pieces
                if(board[row][col] == 'B'){
                    black_pieces.push_back(make_pair(row, col));
                } else if(board[row][col] == 'W'){
                    white_pieces++;
                }

                // King position
                if(board[row][col] == 'K'){
                    stats['K']['R'] = row;
                    stats['K']['C'] = col;
                }
            }
        }


        // King free sides
        if(stats['K']['R'] != 0 && stats['K']['R'] != 8 && stats['K']['C'] != 0 && stats['K']['C'] != 8){
            vector<pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
            for(const auto& dir : directions){
                // King free sides
                if(board[stats['K']['R'] + dir.first][stats['K']['C'] + dir.second] == 'E'){
                    free_sides++;
                } else {
                    blocked_sides++;
                    if(is_citadel(stats['K']['R'] + dir.first, stats['K']['C'] + dir.second) || board[stats['K']['R'] + dir.first][stats['K']['C'] + dir.second] == 'B'){
                        black_blockers++;
                    } else if(board[stats['K']['R'] + dir.first][stats['K']['C'] + dir.second] == 'W'){
                        white_blockers++;
                    } else if(stats['K']['R'] + dir.first == 4 && stats['K']['C'] + dir.second == 4){
                        castle_blockers++;
                    }
                }        int free_sides = 0, blocked_sides = 0, black_blockers = 0, white_blockers = 0, castle_blockers = 0;

            }
        }

        // Black mean distance from king
        for(const auto& piece : black_pieces){
            near_king += abs(piece.first - stats['K']['R']) + abs(piece.second - stats['K']['C']);
        }
        near_king = (8 - ((near_king - black_blockers) / black_pieces.size())) * 100;
        

        pair<int, int> king_position = make_pair(stats['K']['R'], stats['K']['C']);

        stats['B']['P'] = black_pieces.size();
        stats['B']['N'] = near_king;
        stats['B']['T'] = black_checkmate(board, king_position);

        stats['W']['P'] = white_pieces;
        stats['W']['T'] = white_checkmate(board, king_position);

        // stats['K']['R'] riga del re
        // stats['K']['C'] colonna del re
        stats['K']['W'] = king_winning_direction_heatmap[stats['K']['R']][stats['K']['C']];
        stats['K']['F'] = free_sides;
        stats['K']['S'] = blocked_sides;
        stats['K']['B'] = black_blockers;
        stats['K']['W'] = white_blockers;
        stats['K']['C'] = castle_blockers;
        return stats;
    } catch (const exception& e) {
        cerr << "Error in get_stats: " << e.what() << endl;
        throw;  // Rilancia l'eccezione
    }
}

int heuristic_evaluation(const vector<vector<char>>& board, const char& player, const char& game_over) {
    try {

        int points = 0;

        if(game_over == 'W'){
            points = 10000;
        }
        if(game_over == 'B'){
            points = -10000;
        }

        if(points != 0){
            if(player == 'W'){
                return points;
            } else {
                return -points;
            }
        }

        auto stats = get_stats(board);

        // mi sembra bw1 a -80 e bw2 a -10
        int ww1 = 60, ww2 = 50, ww3 = 40, ww4 = 50;
        int bw1 = -60, bw2 = -1, bw3 = -40, bw4 = 25;

        // Material
        int white_material = stats['W']['P'];   // (0-480)
        int black_material = stats['B']['P'];   // (0-960)

        // Position (c'era anche 8 - stats['W']['S'] + ...)
        int white_position = stats['K']['W'];   // (0-200)
        int black_position = stats['B']['N'];   // (0-700)

        // Threats10
        int white_threats = (stats['W']['T']? -100 : 0);    // (-4000/0)
        int black_threats = (stats['B']['T']? -100 : 0);    // (-4000/0)

        // Progress to victory (CATEGORIDA DA RIVEDERE MAGGIORMENTE)
        // white progress forse conviene dare un punteggio per il numero ottimale di bianchi che circonda il re (0, 1, 2, 3, 4)
        int white_progress = stats['K']['F'];   // (0-200)
        int black_progress = stats['K']['B'] + stats['K']['C']; // (0-100)

        // white max 880
        // black max 1760

        
        points =    ww1 * white_material + 
                    ww2 * white_position + 
                    ww3 * white_threats + 
                    ww4 * white_progress +
                    bw1 * black_material +
                    bw2 * black_position +
                    bw3 * black_threats +
                    bw4 * black_progress;

        // Logger::debug(format_board(board));
        // Logger::debug("w1 * " + to_string(white_material) + " + " +
        //               "w2 * " + to_string(white_position) + " + " +
        //               "w3 * " + to_string(white_threats) + " + " +
        //               "w4 * " + to_string(white_progress) + " + " +
        //               "b1 * " + to_string(black_material) + " + " +
        //               "b2 * " + to_string(black_position) + " + " +
        //               "b3 * " + to_string(black_threats) + " + " +
        //               "b4 * " + to_string(black_progress) + " = " + to_string(points));
        // cout << "White Points: (Material) " << ww1 * white_material << " (Position) " << ww2 * white_position << " (Threats) " << ww3 * white_threats << " (Progress) " << ww4 * white_progress << endl;
        // cout << "Black Points: (Material) " << bw1 * black_material << " (Position) " << bw2 * black_position << " (Threats) " << bw3 * black_threats << " (Progress) " << bw4 * black_progress << endl;
        // cout << "Total Points: " << points << endl;

        if(player == 'W'){
            return points;
        } else {
            return -points;
        }
    } catch (const exception& e) {
        cerr << "Error in heuristic_evaluation: " << e.what() << endl;
        throw;  // Rilancia l'eccezione
    }
}

// MINIMAX ALPHA BETA PRUNING con is_max

pair<int, Move> minimax_alpha_beta(const vector<vector<char>>& board, int depth, int alpha, int beta, const char&turn, const char& player) {
    try {
        // Caso base: limite di profondità raggiunto o fine del gioco
        bool game_over = is_game_over(board);
        if (depth == 0 || game_over != 'N') {
            return {heuristic_evaluation(board, player, game_over), Move()};  // Restituisce punteggio e nessuna mossa usando l'euristica
        }

        bool is_max = (turn == player);  // Se il turno è del giocatore, è una fase di massimizzazione
        Move best_move;

        if (is_max) {
            int max_eval = std::numeric_limits<int>::min();
            for (const Move& move : generate_all_possible_moves(board, turn)) {
                // Applica la mossa per il giocatore max
                // cout << "Move from: " << move.from.first << " " << move.from.second << " to: " << move.to.first << " " << move.to.second << endl;
                vector<vector<char>> new_board = apply_move(board, move);
                auto [eval, _] = minimax_alpha_beta(new_board, depth - 1, alpha, beta, get_opposite_turn(turn), player);
                
                if (eval > max_eval) {
                    max_eval = eval;
                    best_move = move;
                }
                
                alpha = max(alpha, eval);
                
                // Potatura Beta
                if (beta <= alpha) {
                    break;
                }
            }
            // cout << "Best move (MAX): " << best_move.from.first << " " << best_move.from.second << " to: " << best_move.to.first << " " << best_move.to.second << endl;
            return {max_eval, best_move};
        } else {
            int min_eval = std::numeric_limits<int>::max();
            for (const Move& move : generate_all_possible_moves(board, turn)) {
                // Applica la mossa per il giocatore min
                vector<vector<char>> new_board = apply_move(board, move);
                auto [eval, _] = minimax_alpha_beta(new_board, depth - 1, alpha, beta, get_opposite_turn(turn), player);
                
                if (eval < min_eval) {
                    min_eval = eval;
                    best_move = move;
                }
                
                beta = min(beta, eval);
                
                // Potatura Alpha
                if (beta <= alpha) {
                    break;
                }
            }
            // cout << "Best move (min): " << best_move.from.first << " " << best_move.from.second << " to: " << best_move.to.first << " " << best_move.to.second << endl;
            return {min_eval, best_move};
        }
    } catch (const exception& e) {
        cerr << "Errore in minimax_alpha_beta: " << e.what() << endl;
        throw;
    }
}

pair<int, Move> minimax_alpha_beta_fast(const vector<vector<char>>& board, int depth, int alpha, int beta, const char& turn, const char& player, int cut_size) {
    try {
        // Caso base: limite di profondità raggiunto o fine del gioco
        char game_over = is_game_over(board);
        if (depth == 0 || game_over != 'N') {
            return {heuristic_evaluation(board, player, game_over), Move()};  // Restituisce punteggio e nessuna mossa usando l'euristica
        }

        Move best_move;
        bool is_max = (turn == player);  // Se il turno è del giocatore, è una fase di massimizzazione

        // Generazione delle mosse
        std::vector<Move> generated_moves = generate_all_possible_moves(board, turn);


        // Seleziona le migliori mosse basate sull'euristica
        std::vector<std::pair<int, Move>> evaluated_moves; // Lista di valutazioni e mosse
        for (const Move& move : generated_moves) {
            vector<vector<char>> new_board = apply_move(board, move); // Applica la mossa per calcolare l'euristica
            char game_over = is_game_over(new_board);
            int score = heuristic_evaluation(new_board, player, game_over); // Valuta il punteggio
            evaluated_moves.push_back({score, move}); // Salva punteggio e mossa
        }

        // Ordina le mosse basandosi solo sul punteggio
        if (is_max) {
            std::sort(evaluated_moves.begin(), evaluated_moves.end(), 
                      [](const auto& a, const auto& b) { return a.first > b.first; });
        } else {
            std::sort(evaluated_moves.begin(), evaluated_moves.end(), 
                      [](const auto& a, const auto& b) { return a.first < b.first; });
        }

        // Mantieni solo le prime "cut_size" mosse
        if (evaluated_moves.size() > cut_size) {
            evaluated_moves.resize(cut_size);
        }

        // Ora esegui il Minimax con Alpha-Beta sulle migliori mosse
        if (is_max) {
            int max_eval = std::numeric_limits<int>::min();
            for (const auto& [score, move] : evaluated_moves) {
                vector<vector<char>> new_board = apply_move(board, move);
                auto [eval, dummy] = minimax_alpha_beta_fast(new_board, depth - 1, alpha, beta, get_opposite_turn(turn), player, cut_size);
                
                if (eval > max_eval) {
                    max_eval = eval;
                    best_move = move;
                }
                
                alpha = std::max(alpha, eval);
                if (beta <= alpha) { // Potatura Beta
                    break;
                }
            }
            return {max_eval, best_move};
        } else {
            int min_eval = std::numeric_limits<int>::max();
            for (const auto& [score, move] : evaluated_moves) {
                vector<vector<char>> new_board = apply_move(board, move);
                auto [eval, dummy] = minimax_alpha_beta_fast(new_board, depth - 1, alpha, beta, get_opposite_turn(turn), player, cut_size);
                
                if (eval < min_eval) {
                    min_eval = eval;
                    best_move = move;
                }
                
                beta = std::min(beta, eval);
                if (beta <= alpha) { // Potatura Alpha
                    break;
                }
            }
            return {min_eval, best_move};
        }
    } catch (const exception& e) {
        cerr << "Errore in minimax_alpha_beta: " << e.what() << endl;
        throw;
    }
}