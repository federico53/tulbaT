#include "headers/MinMax.h"
#include <thread>

using namespace std;

int COUNTER = 0;

std::map<char, std::map<char, int>> get_stats(const vector<vector<char>>& board) {
    try {
        map<char, map<char, int>> stats;
        int black_pieces = 0, white_pieces = 0;
        int bsp = 0, wsp = 0;
        int free_sides = 0, blocked_sides = 0, black_blockers = 0, white_blockers = 0, castle_blockers = 0;
        int near_king = 0;

        for(int row = 0; row < 9; ++row){
            for(int col = 0; col < 9; ++col){
                
                // Pieces
                if(board[row][col] == 'B'){
                    black_pieces++;
                } else if(board[row][col] == 'W'){
                    white_pieces++;
                }

                // Starting positions white
                if(row == 2 && col == 4 && board[row][col] == 'W'){
                    wsp++;
                }
                if(row == 3 && col == 4 && board[row][col] == 'W'){
                    wsp++;
                }
                if(row == 4 && col == 2 && board[row][col] == 'W'){
                    wsp++;
                }
                if(row == 4 && col == 3 && board[row][col] == 'W'){
                    wsp++;
                }
                if(row == 4 && col == 6 && board[row][col] == 'W'){
                    wsp++;
                }
                if(row == 4 && col == 7 && board[row][col] == 'W'){
                    wsp++;
                }
                if(row == 5 && col == 4 && board[row][col] == 'W'){
                    wsp++;
                }
                if(row == 6 && col == 4 && board[row][col] == 'W'){
                    wsp++;
                }

                // Starting positions black
                if(row == 0 && col == 3 && board[row][col] == 'B'){
                    bsp++;
                }
                if(row == 0 && col == 4 && board[row][col] == 'B'){
                    bsp++;
                }
                if(row == 0 && col == 5 && board[row][col] == 'B'){
                    bsp++;
                }
                if(row == 1 && col == 4 && board[row][col] == 'B'){
                    bsp++;
                }
                if(row == 3 && col == 0 && board[row][col] == 'B'){
                    bsp++;
                }
                if(row == 4 && col == 0 && board[row][col] == 'B'){
                    bsp++;
                }
                if(row == 5 && col == 0 && board[row][col] == 'B'){
                    bsp++;
                }
                if(row == 4 && col == 1 && board[row][col] == 'B'){
                    bsp++;
                }
                if(row == 3 && col == 8 && board[row][col] == 'B'){
                    bsp++;
                }
                if(row == 4 && col == 8 && board[row][col] == 'B'){
                    bsp++;
                }
                if(row == 5 && col == 8 && board[row][col] == 'B'){
                    bsp++;
                }
                if(row == 4 && col == 7 && board[row][col] == 'B'){
                    bsp++;
                }
                if(row == 8 && col == 3 && board[row][col] == 'B'){
                    bsp++;
                }
                if(row == 8 && col == 4 && board[row][col] == 'B'){
                    bsp++;
                }
                if(row == 8 && col == 5 && board[row][col] == 'B'){
                    bsp++;
                }
                if(row == 7 && col == 4 && board[row][col] == 'B'){
                    bsp++;
                }

                // King position
                if(board[row][col] == 'K'){
                    stats['K']['R'] = row;
                    stats['K']['C'] = col;
                }

            }
        }

        // King free sides + near_king
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
                    } else if(is_castle(stats['K']['R'] + dir.first, stats['K']['C'] + dir.second)){
                        castle_blockers++;
                    }
                }        int free_sides = 0, blocked_sides = 0, black_blockers = 0, white_blockers = 0, castle_blockers = 0;


                // Near king perpendicularl
                for(int i = 1; i < 3; ++i){
                    if(stats['K']['R'] + dir.first * i >= 0 && stats['K']['R'] + dir.first * i < 9 && stats['K']['C'] + dir.second * i >= 0 && stats['K']['C'] + dir.second * i < 9)
                        if(board[stats['K']['R'] + dir.first * i][stats['K']['C'] + dir.second * i] == 'B'){
                            // cambiato (5 - i)
                            near_king += (4 - 2 * (i - 1));                            
                        }
                }


            }
            // Near king diagonal
            vector<pair<int,int>> diagonal = {{1,1}, {1,-1}, {-1,1}, {-1,-1}};
            for(const auto& dir : diagonal){
                if(stats['K']['R'] + dir.first >= 0 && stats['K']['R'] + dir.first < 9 && stats['K']['C'] + dir.second >= 0 && stats['K']['C'] + dir.second < 9){
                    if(board[stats['K']['R'] + dir.first][stats['K']['C'] + dir.second] == 'B'){
                        // 2
                        near_king += 3;
                    }
                }
            }
        }
        
        stats['B']['P'] = black_pieces;
        stats['B']['S'] = bsp;
        stats['B']['N'] = near_king;
        stats['B']['T'] = black_checkmate(board);

        stats['W']['P'] = white_pieces;
        stats['W']['S'] = wsp;
        stats['W']['T'] = white_checkmate(board);

        stats['K']['W'] = king_winning_direction_score(board);
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

// DA IGNORARE
tuple<int, int> get_points(const vector<vector<char>>& board){
    try{
        map<char, map<char, int>> stats = get_stats(board);
        int black, white; 

        // -------------------- BLACK --------------------
        int w1 = 125, w2 = 50, w3 = 10, w4 = 100;
        int material, position, threats, progress;

        // Material (max 8)
        // Pezzi del nero - pezzi del bianco
        material = (8 - stats['W']['P']);

        // Position (max 28 ma meno)
        // Neri fuori dalla casa iniziale + punteggio vicinanza a re + controllo dei corridoi + ...
        position = stats['K']['N'];

        // Threats
        // Se il bianco puo fare checkmate + 
        threats = (black_checkmate(board)? -100 : 0);

        // Progress to victory (max  10)
        // Lati del re bloccati + vittoria + checkmate
        if(is_game_over(board) == 'B'){
            return {4000, 0};
        }
        progress = (white_checkmate(board)? 7 : 0);
        progress += stats['K']['B'] + stats['K']['C'];

        black =  w1 * material + w2 * position + w3 * threats + w4 * progress;

        // -------------------- WHITE --------------------
        w1 = 10, w2 = 10, w3 = 20, w4 = 20;
        material, position, threats, progress;

        // Material
        // Pezzi del bianco - pezzi del nero
        material = (8 - stats['W']['P']) - (16 - stats['B']['P']);

        // Position
        // Bianchi fuori dalla casa iniziale + uscite del re + ...
        position = (8 - stats['W']['S']) + king_winning_direction_score(board);

        // Threats
        // Se il nero puo fare checkmate + 
        threats = (white_checkmate(board)? -100 : 0);

        // Progress to victory
        // Lati del re bloccati + vittoria + checkmate
        if(is_game_over(board) == 'W'){
            return {0, 10000};
        }
        progress = (black_checkmate(board)? 200 : 0);
        progress += stats['K']['F'] + stats['K']['W'];

        white = w1 * material + w2 * position + w3 * threats + w4 * progress;
        
        return {black, white};
    } catch (const exception& e) {
        cerr << "Error in get_points: " << e.what() << endl;
        throw;  // Rilancia l'eccezione
    }
}

int heuristic_evaluation(const vector<vector<char>>& board, const char& player) {
    try {
        // auto points = get_points(board);
        // int black = get<0>(points);
        // int white = get<1>(points);
        // if(player == 'W'){
        //     return white - black;
        // } else {
        //     return black - white;
        // }

        int points = 0;

        if(is_game_over(board) == 'W'){
            points = 10000;
        }
        if(is_game_over(board) == 'B'){
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
        int ww1 = 60, ww2 = 40, ww3 = 60, ww4 = 50;
        int bw1 = -70, bw2 = -15, bw3 = -40, bw4 = -50;

        // Material
        int white_material = stats['W']['P'];
        int black_material = stats['B']['P'];

        // Position (c'era anche 8 - stats['W']['S'] + ...)
        int white_position = stats['K']['W'];
        int black_position = stats['B']['N'];

        // Threats10
        int white_threats = (stats['W']['T']? -100 : 0);
        int black_threats = (stats['B']['T']? -100 : 0);

        // Progress to victory
        int white_progress = stats['K']['F'] + stats['K']['W'];
        int black_progress = stats['K']['B'] + stats['K']['C'];

        
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
        if (depth == 0 || is_game_over(board) != 'N') {
            return {heuristic_evaluation(board, player), Move()};  // Restituisce punteggio e nessuna mossa usando l'euristica
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
        if (depth == 0 || is_game_over(board) != 'N') {
            return {heuristic_evaluation(board, player), Move()};  // Restituisce punteggio e nessuna mossa usando l'euristica
        }

        Move best_move;
        bool is_max = (turn == player);  // Se il turno è del giocatore, è una fase di massimizzazione

        // Generazione delle mosse
        std::vector<Move> generated_moves = generate_all_possible_moves(board, turn);

        // Seleziona le migliori mosse basate sull'euristica
        std::vector<std::pair<int, Move>> evaluated_moves; // Lista di valutazioni e mosse
        for (const Move& move : generated_moves) {
            vector<vector<char>> new_board = apply_move(board, move); // Applica la mossa per calcolare l'euristica
            int score = heuristic_evaluation(new_board, player); // Valuta il punteggio
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

#include <future>
#include <atomic>
#include <chrono>
#include <iostream>

std::atomic<bool> stop_threads(false);


pair<int, Move> minimax_alpha_beta_fast_with_thread(const vector<vector<char>>& board, int depth, int alpha, int beta, const char& turn, const char& player, int cut_size) {
    try {
        // Caso base: limite di profondità raggiunto, fine del gioco o richiesta di terminazione
        if (depth == 0 || is_game_over(board) != 'N' || stop_threads) {
            return {heuristic_evaluation(board, player), Move()};  // Restituisce punteggio e nessuna mossa usando l'euristica
        }

        Move best_move;
        bool is_max = (turn == player);  // Se il turno è del giocatore, è una fase di massimizzazione

        // Generazione delle mosse
        std::vector<Move> generated_moves = generate_all_possible_moves(board, turn);

        // Seleziona le migliori mosse basate sull'euristica
        std::vector<std::pair<int, Move>> evaluated_moves; // Lista di valutazioni e mosse
        for (const Move& move : generated_moves) {
            if (stop_threads) break; // Controlla la richiesta di terminazione
            vector<vector<char>> new_board = apply_move(board, move); // Applica la mossa per calcolare l'euristica
            int score = heuristic_evaluation(new_board, player); // Valuta il punteggio
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
                if (stop_threads) break; // Controlla la richiesta di terminazione
                vector<vector<char>> new_board = apply_move(board, move);
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
            for (const auto& [score, move] : evaluated_moves) {
                if (stop_threads) break; // Controlla la richiesta di terminazione
                vector<vector<char>> new_board = apply_move(board, move);
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

pair<int, Move> run_minimax_with_threads(const vector<vector<char>>& board, int depth, const char& turn, const char& player, int cut_size) {
    stop_threads = false;

    // Future per i risultati dei thread
    auto start_time_depth = std::chrono::steady_clock::now();
    std::future<pair<int, Move>> result_depth = std::async(std::launch::async, run_minimax, board, depth, turn, player, cut_size);
    auto start_time_depth_plus_1 = std::chrono::steady_clock::now();
    std::future<pair<int, Move>> result_depth_plus_1 = std::async(std::launch::async, run_minimax, board, depth + 1, turn, player, cut_size);
    auto start_time_depth_plus_2 = std::chrono::steady_clock::now();
    std::future<pair<int, Move>> result_depth_plus_2 = std::async(std::launch::async, run_minimax, board, depth + 2, turn, player, cut_size);

    // Timer di 58 secondi
    auto start_time = std::chrono::steady_clock::now();
    auto end_time = start_time + std::chrono::seconds(58);

    // Raccogliere i risultati
    pair<int, Move> best_result = {std::numeric_limits<int>::min(), Move()};

    auto check_and_update_result = [&](std::future<pair<int, Move>>& result_future, int depth_label, auto start_time) {
        if (result_future.valid() && result_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            try {
                auto result = result_future.get();
                auto end_time = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
                std::cout << "Thread of depth " << depth_label << " returned with score: " << result.first << " in " << duration / 1000.0 << " seconds" << std::endl;
                if (result.first > best_result.first) {
                    best_result = result;
                }
                return true;
            } catch (const std::exception& e) {
                std::cerr << "Error in " << depth_label << ": " << e.what() << std::endl;
            }
        }
        return false;
    };

    bool res1 = false, res2 = false, res3 = false;

    while (std::chrono::steady_clock::now() < end_time && (!res1 || !res2 || !res3)) {
        if (!res1)
            res1 = check_and_update_result(result_depth, depth, start_time_depth);
        if (!res2)
            res2 = check_and_update_result(result_depth_plus_1, depth+1, start_time_depth_plus_1);
        if (!res3)
            res3 = check_and_update_result(result_depth_plus_2, depth+2, start_time_depth_plus_2);

        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Attendi un po' prima di ricontrollare
    }

    stop_threads = true;

    // Forza la raccolta dei risultati rimanenti

    if (!res1)
        check_and_update_result(result_depth, depth, start_time_depth);
    if (!res2)
        check_and_update_result(result_depth_plus_1, depth+1, start_time_depth_plus_1);
    if (!res3)
        check_and_update_result(result_depth_plus_2, depth+2, start_time_depth_plus_2);

    std:cout << "We should have finished, returning best result" << std::endl;

    return best_result;
}