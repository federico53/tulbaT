#include "headers/MinMax.h"
#include <thread>

using namespace std;

int COUNTER = 0;

std::map<char, std::map<char, int>> get_stats(const vector<vector<char>>& board) {
    try {
        map<char, map<char, int>> stats;
        vector<pair<int, int>> black_pieces;
        int white_pieces = 0;
        int free_sides = 0, blocked_sides = 0, black_blockers = 0, white_blockers = 0, castle_blockers = 0;
        float near_king_black = 0, near_king_white = 0;
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
                }

                // // Near king perpendicularl
                // int i = 2;
                // if(stats['K']['R'] + dir.first * i >= 0 && stats['K']['R'] + dir.first * i < 9 && stats['K']['C'] + dir.second * i >= 0 && stats['K']['C'] + dir.second * i < 9){
                //     if(board[stats['K']['R'] + dir.first * i][stats['K']['C'] + dir.second * i] == 'W'){
                //         // cambiato (5 - i)
                //         near_king_white += i;                           
                //     }
                // }
                // // for(int i = 1; i < 3; ++i){
                // // }
            }

            // Near king white diagonal
            vector<pair<int,int>> diagonal = {{1,1}, {1,-1}, {-1,1}, {-1,-1}};
            for(const auto& dir : diagonal){
                if(stats['K']['R'] + dir.first >= 0 && stats['K']['R'] + dir.first < 9 && stats['K']['C'] + dir.second >= 0 && stats['K']['C'] + dir.second < 9){
                    if(board[stats['K']['R'] + dir.first][stats['K']['C'] + dir.second] == 'W'){
                        // 2
                        near_king_white += 1;
                    }
                }
            }
        }

        // Black mean distance from king
        for(const auto& piece : black_pieces){
            near_king_black += abs(piece.first - stats['K']['R']) + abs(piece.second - stats['K']['C']);
        }
        near_king_black = (8 - ((near_king_black- black_blockers) / black_pieces.size())) * 100;
        

        pair<int, int> king_position = make_pair(stats['K']['R'], stats['K']['C']);

        stats['B']['P'] = black_pieces.size();
        stats['B']['N'] = near_king_black;
        stats['B']['T'] = black_checkmate(board, king_position);

        stats['W']['P'] = white_pieces;
        stats['W']['N'] = near_king_white;
        stats['W']['T'] = white_checkmate(board, king_position);

        // stats['K']['R'] riga del re
        // stats['K']['C'] colonna del re
        stats['K']['W'] = king_winning_direction_heatmap[stats['K']['R']][stats['K']['C']] * 2;
        stats['K']['F'] = free_sides;
        stats['K']['S'] = blocked_sides;
        stats['K']['B'] = black_blockers;
        stats['K']['W'] = white_blockers;
        stats['K']['T'] = castle_blockers;
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
        int ww1 = 60, ww2 = 100, ww3 = 40, ww4 = 50;
        int bw1 = -60, bw2 = -2, bw3 = -40, bw4 = 25;

        // Material
        int white_material = stats['W']['P'];   // (0-480)
        int black_material = stats['B']['P'];   // (0-960)

        // Position (c'era anche 8 - stats['W']['S'] + ...)
        int white_position = stats['K']['W'] + stats['W']['N'];   // (0-800)
        int black_position = stats['B']['N'];   // (0-1400)

        // Threats10
        int white_threats = (stats['W']['T']? -100 : 0);    // (-4000/0)
        int black_threats = (stats['B']['T']? -100 : 0);    // (-4000/0)

        // Progress to victory (CATEGORIDA DA RIVEDERE MAGGIORMENTE)
        int white_progress = stats['K']['F'];   // (0-200)
        int black_progress = stats['K']['B'] + stats['K']['T']; // (0-100)

        // white max 1480
        // black max 2460

        
        points =    ww1 * white_material + 
                    ww2 * white_position + 
                    ww3 * white_threats + 
                    ww4 * white_progress +
                    bw1 * black_material +
                    bw2 * black_position +
                    bw3 * black_threats +
                    bw4 * black_progress;

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
        std::vector<std::tuple<int, Move, vector<vector<char>>>> evaluated_moves; // Lista di valutazioni, mosse e nuove board
        for (const Move& move : generated_moves) {
            vector<vector<char>> new_board = apply_move(board, move); // Applica la mossa per calcolare l'euristica
            char game_over = is_game_over(new_board);
            int score = heuristic_evaluation(new_board, player, game_over); // Valuta il punteggio
            evaluated_moves.push_back({score, move, new_board}); // Salva punteggio, mossa e nuova board
        }

        // Ordina le mosse basandosi solo sul punteggio
        if (is_max) {
            std::sort(evaluated_moves.begin(), evaluated_moves.end(), 
                      [](const auto& a, const auto& b) { return get<0>(a) > get<0>(b); });
        } else {
            std::sort(evaluated_moves.begin(), evaluated_moves.end(), 
                      [](const auto& a, const auto& b) { return get<0>(a) < get<0>(b); });
        }

        // Mantieni solo le prime "cut_size" mosse
        if (evaluated_moves.size() > cut_size) {
            evaluated_moves.resize(cut_size);
        }

        // Ora esegui il Minimax con Alpha-Beta sulle migliori mosse
        if (is_max) {
            int max_eval = std::numeric_limits<int>::min();
            for (const auto& [score, move, new_board] : evaluated_moves) {
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
            for (const auto& [score, move, new_board] : evaluated_moves) {
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

#include <future>
#include <atomic>
#include <chrono>
#include <iostream>

std::atomic<bool> stop_threads(false);


pair<int, Move> minimax_alpha_beta_fast_with_thread(const vector<vector<char>>& board, int depth, int alpha, int beta, const char& turn, const char& player, int cut_size) {
    try {
        // Caso base: limite di profondità raggiunto, fine del gioco o richiesta di terminazione
        char game_over = is_game_over(board);
        if (depth == 0 || game_over != 'N' || stop_threads) {
            return {heuristic_evaluation(board, player, game_over), Move()};  // Restituisce punteggio e nessuna mossa usando l'euristica
        }

        Move best_move;
        bool is_max = (turn == player);  // Se il turno è del giocatore, è una fase di massimizzazione

        // Generazione delle mosse
        std::vector<Move> generated_moves = generate_all_possible_moves(board, turn);

        // Seleziona le migliori mosse basate sull'euristica
        std::vector<std::tuple<int, Move, vector<vector<char>>>> evaluated_moves; // Lista di valutazioni e mosse
        for (const Move& move : generated_moves) {
            if (stop_threads) break; // Controlla la richiesta di terminazione
            vector<vector<char>> new_board = apply_move(board, move); // Applica la mossa per calcolare l'euristica
            char game_over = is_game_over(new_board);
            int score = heuristic_evaluation(new_board, player, game_over); // Valuta il punteggio
            evaluated_moves.push_back({score, move, new_board}); // Salva punteggio e mossa
        }

        // Ordina le mosse basandosi solo sul punteggio
        if (is_max) {
            std::sort(evaluated_moves.begin(), evaluated_moves.end(), 
                      [](const auto& a, const auto& b) { return get<0>(a) > get<0>(b); });
        } else {
            std::sort(evaluated_moves.begin(), evaluated_moves.end(), 
                      [](const auto& a, const auto& b) { return get<0>(a) < get<0>(b); });
        }

        // Mantieni solo le prime "cut_size" mosse
        if (evaluated_moves.size() > cut_size) {
            evaluated_moves.resize(cut_size);
        }

        // Ora esegui il Minimax con Alpha-Beta sulle migliori mosse
        if (is_max) {
            int max_eval = std::numeric_limits<int>::min();
            for (const auto& [score, move, new_board] : evaluated_moves) {
                if (stop_threads) break; // Controlla la richiesta di terminazione
                auto [eval, dummy] = minimax_alpha_beta_fast_with_thread(new_board, depth - 1, alpha, beta, get_opposite_turn(turn), player, cut_size);
                
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
            for (const auto& [score, move, new_board] : evaluated_moves) {
                if (stop_threads) break; // Controlla la richiesta di terminazione
                auto [eval, dummy] = minimax_alpha_beta_fast_with_thread(new_board, depth - 1, alpha, beta, get_opposite_turn(turn), player, cut_size);
                
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



pair<int, Move> run_minimax(const vector<vector<char>>& board, int depth, const char& turn, const char& player, int cut_size) {
    if (stop_threads) {
        return {std::numeric_limits<int>::min(), Move()};
    }
    return minimax_alpha_beta_fast_with_thread(board, depth, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), turn, player, cut_size);
}

pair<int, Move> run_minimax_with_threads(const vector<vector<char>>& board, int depth, const char& turn, const char& player, int cut_size, int time) {
    stop_threads = false;

    // Future per i risultati dei thread
    auto start_time_depth_minus_2 = std::chrono::steady_clock::now();
    std::future<pair<int, Move>> result_depth_minus_2 = std::async(std::launch::async, run_minimax, board, depth - 2, turn, player, cut_size);
    auto start_time_depth_minus_1 = std::chrono::steady_clock::now();
    std::future<pair<int, Move>> result_depth_minus_1 = std::async(std::launch::async, run_minimax, board, depth - 1, turn, player, cut_size);
    auto start_time_depth = std::chrono::steady_clock::now();
    std::future<pair<int, Move>> result_depth = std::async(std::launch::async, run_minimax, board, depth, turn, player, cut_size);
    auto start_time_depth_plus_1 = std::chrono::steady_clock::now();
    std::future<pair<int, Move>> result_depth_plus_1 = std::async(std::launch::async, run_minimax, board, depth + 1, turn, player, cut_size);

    // Timer di (time-2) secondi per stare larghi
    auto start_time = std::chrono::steady_clock::now();
    auto end_time = start_time + std::chrono::seconds(time-2);

    // Raccogliere i risultati
    pair<int, Move> best_result = {std::numeric_limits<int>::min(), Move()};

    auto check_and_update_result = [&](std::future<pair<int, Move>>& result_future, int depth_label, auto start_time) {
        if (result_future.valid() && result_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            try {
                auto result = result_future.get();
                auto end_time = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
                std::cout << "Thread of depth " << depth_label << " returned with score: " << result.first << " in " << duration / 1000.0 << " seconds" << std::endl;
                
                best_result = result;
            
                return true;
            } catch (const std::exception& e) {
                std::cerr << "Error in " << depth_label << ": " << e.what() << std::endl;
            }
        }
        return false;
    };

    bool res_minus_2 = false, res_minus_1 = false, res = false, res_plus_1 = false;
    std::future<pair<int, Move>> result_depth_plus_2;
    bool res_plus_2 = false;
    std::chrono::steady_clock::time_point start_time_depth_plus_2;

    while (std::chrono::steady_clock::now() < end_time && (!res_minus_2 || !res_minus_1 || !res || !res_plus_1 || !res_plus_2)) {
        if (!res_minus_2) {
            res_minus_2 = check_and_update_result(result_depth_minus_2, depth - 2, start_time_depth_minus_2);
            if (res_minus_2) {
                start_time_depth_plus_2 = std::chrono::steady_clock::now();
                result_depth_plus_2 = std::async(std::launch::async, run_minimax, board, depth + 2, turn, player, cut_size);
            }
        }
        if (!res_minus_1)
            res_minus_1 = check_and_update_result(result_depth_minus_1, depth - 1, start_time_depth_minus_1);
        if (!res)
            res = check_and_update_result(result_depth, depth, start_time_depth);
        if (!res_plus_1)
            res_plus_1 = check_and_update_result(result_depth_plus_1, depth + 1, start_time_depth_plus_1);
        if (result_depth_plus_2.valid() && !res_plus_2)
            res_plus_2 = check_and_update_result(result_depth_plus_2, depth + 2, start_time_depth_plus_2);

        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Attendi un po' prima di ricontrollare
    }

    stop_threads = true;

    // Forza la raccolta dei risultati rimanenti
    if (!res_minus_2)
        check_and_update_result(result_depth_minus_2, depth - 2, start_time_depth_minus_2);
    if (!res_minus_1)
        check_and_update_result(result_depth_minus_1, depth - 1, start_time_depth_minus_1);
    if (!res)
        check_and_update_result(result_depth, depth, start_time_depth);
    if (!res_plus_1)
        check_and_update_result(result_depth_plus_1, depth + 1, start_time_depth_plus_1);
    if (!res_plus_2 && result_depth_plus_2.valid())
        check_and_update_result(result_depth_plus_2, depth + 2, start_time_depth_plus_2);

    std::cout << "We should have finished, returning best result" << std::endl;

    // Restituisce il valore del thread con la profondità maggiore tra quelli che hanno finito
    return best_result;
}