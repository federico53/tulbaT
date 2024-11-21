#include "MinMax.h"

using namespace std;


// WHITE HEURISTIC
/* int heuristic_white(const vector<vector<char>>& board) {
    try {
        // 1. Posizione del re - TODO
        // 2. Posizione del re ai lati - TODO
        
        // 3. Scacco matto al nero
        int points_king_checkmate = 0;
        if (black_checkmate(board)) {
            points_king_checkmate = 100;
        }

        // 4. Differenza tra i pezzi mangiati dal nero e quelli persi dal bianco
        int white_lost = 8 - count_pieces(board, 'W');
        int black_eaten = 16 - count_pieces(board, 'B');
        int diff_between_white_black = 1600 * (black_eaten - white_lost) * 200;

        // 5. Nero mangiato
        int score_black_eaten = 5000 * black_eaten;

        // 6. Il re non è bloccato nel quadrante - TODO
        int score_king_not_blocked_quadrant = 0;

        // 7. Il nero non può fare scacco matto in una mossa
        int score_king_not_checkmated_infuture = 0;
        if (!black_can_checkmate_in_future(board)) {
            score_king_not_checkmated_infuture = 20000;
        }

        // 8. Il re non è in scacco matto dal nero
        int score_king_not_checkmated = 0;
        if (!white_checkmate(board)) {
            score_king_not_checkmated = 40000;
        }

        // 9. Se il nero non ha vinto
        int white_didnt_win_points = 0;
        if (is_game_over(board) != "black") {
            white_didnt_win_points = 80000;
        }

        // 10. Se il bianco ha vinto
        int black_won_points = 0;
        if (is_game_over(board) == "white") {
            black_won_points = 160000;
        }

        // Calcolo finale del punteggio
        return points_king_checkmate + diff_between_white_black + score_black_eaten + 
               score_king_not_blocked_quadrant + score_king_not_checkmated_infuture + 
               score_king_not_checkmated + white_didnt_win_points + black_won_points;
    } catch (const exception& e) {
        cerr << "Error in heuristic_white: " << e.what() << endl;
        return 0;  // Gestione dell'errore
    }
} */

int heuristic_white(const vector<vector<char>>& board) {

    int sum = 0;

    if (is_game_over(board) == "white") {
        sum += 1000000;
    }

    if (is_game_over(board) == "black") {
        sum -= 2000000;
    }
    
    int black_eaten = 16 - count_pieces(board, 'B');
    sum += black_eaten * 500;

    int points_king_checkmate = 0;
        if (black_checkmate(board)) {
            points_king_checkmate = 1000;
        }
    sum += points_king_checkmate;


    return sum;
    
}



// BLACK HEURISTIC

int calculate_black_starting_positions_points(const vector<vector<char>>& board) {
    try {
        int points = 0;

        // Posizioni di partenza per i pezzi neri
        vector<pair<int, int>> top_positions = {{0, 4}, {8, 4}, {4, 0}, {4, 8}};
        vector<pair<int, int>> medium_positions = {{1, 4}, {7, 4}, {4, 1}, {4, 7}};
        vector<pair<int, int>> low_positions = {{0, 3}, {0, 5}, {3, 0}, {5, 0}, {3, 8}, {5, 8}, {8, 3}, {8, 5}};

        // Assegna punti in base alla posizione dei pezzi neri
        for (const auto& pos : top_positions) {
            int row = pos.first;
            int col = pos.second;
            if (board[row][col] == 'B') {
                points += 3;
            }
        }

        for (const auto& pos : medium_positions) {
            int row = pos.first;
            int col = pos.second;
            if (board[row][col] == 'B') {
                points += 2;
            }
        }

        for (const auto& pos : low_positions) {
            int row = pos.first;
            int col = pos.second;
            if (board[row][col] == 'B') {
                points += 1;
            }
        }

        return points;
    } catch (const exception& e) {
        cerr << "Error in calculate_black_starting_positions_points: " << e.what() << endl;
        throw;  // Rilancia l'eccezione
    }
}

int heuristic_black(const std::vector<std::vector<char>>& board) {
    try {
        // 1. Punti per le posizioni di partenza dei pezzi neri
        int black_starting_positions_points = calculate_black_starting_positions_points(board);

        // 2. Differenza tra i pezzi bianchi mangiati e i pezzi neri persi
        int white_eaten = 8 - count_pieces(board, 'W');
        int black_lost = 16 - count_pieces(board, 'B');
        int diff_between_white_black = 480 + (white_eaten - black_lost) * 30;

        // 3. Punti per ogni pezzo bianco mangiato
        int points_white_eaten = white_eaten * 750;

        // 4. Se il re non può dare scacco matto in una mossa
        int king_not_checkmate_inonemove = 0;
        if (!king_can_checkmate_in_future(board)) {
            king_not_checkmate_inonemove = 6750;
        }

        // 5. Se il re è in scacco matto
        int king_not_checkmate_points = 0;
        if (white_checkmate(board)) {
            king_not_checkmate_points = 13500;
        }

        // 6. Se il nero non è in scacco matto
        int black_checkmate_points = 0;
        if (!black_checkmate(board)) {
            black_checkmate_points = 27000;
        }

        // 7. Se il bianco non ha vinto
        int white_didnt_win_points = 0;
        if (is_game_over(board) != "white") {
            white_didnt_win_points = 54000;
        }

        // 8. Se il nero ha vinto
        int black_won_points = 0;
        if (is_game_over(board) == "black") {
            black_won_points = 108000;
        }

        return black_starting_positions_points + diff_between_white_black + points_white_eaten + king_not_checkmate_inonemove + king_not_checkmate_points + black_checkmate_points + white_didnt_win_points + black_won_points;
    } catch (const std::exception& e) {
        std::cerr << "Error in heuristic_black: " << e.what() << std::endl;
        throw; // Rilancia l'eccezione
    }
}

// HEURISTIC

int heuristic(const vector<vector<char>>& board, const char& turn) {
    // Funzione euristica per il calcolo del punteggio in base al turno
    if (turn != 'W') {
        return heuristic_white(board);
    } else {
        return heuristic_black(board);
    }
}

int heuristic_evaluation(const vector<vector<char>>& board, const char& turn, const char& player) {
    /*
    Funzione di valutazione euristica per lo stato attuale della scacchiera
    Args:
        board: stato attuale della scacchiera
        turn: turno corrente (black/white)
        player: colore del giocatore (black/white)
    Returns:
        score: punteggio di valutazione per lo stato della scacchiera attuale
                negativo se è una mossa migliore per min, positivo se è una mossa migliore per max
    */
    
    // Poiché il turno è già stato cambiato nella funzione minimax, per scegliere la giusta euristica
    // bisogna ragionare come se fossimo nel prossimo turno
    if (turn != player) {
        return heuristic(board, turn);
    } else {
        return -heuristic(board, turn);
    }
}



// MINIMAX ALPHA BETA PRUNING

pair<int, Move> minimax_alpha_beta(const vector<vector<char>>& board, int depth, int alpha, int beta, const char& turn, const char& player) {
    try {
        // Caso base: limite di profondità raggiunto o fine del gioco
        //stampiammo tutti i parametri ricevuti per controllare se sono corretti

        if (depth == 0 || is_game_over(board) != "") {
            //printiamo qualcosa per capire se siamo entrati in questo if
            return {heuristic_evaluation(board, turn, player), Move()};  // Restituisce punteggio e nessuna mossa usando l'euristica
        }

        Move best_move;
        bool is_max = (turn == player);  // Se il turno è del giocatore, è una fase di massimizzazione

        if (is_max) {
            int max_eval = std::numeric_limits<int>::min();
            for (const Move& move : generate_all_possible_moves(board, player)) {
                // Applica la mossa per il giocatore max
                //Logger::info("Applying move MAX: " + to_string(move.from.first) + to_string(move.from.second) + " to " + to_string(move.to.first) + to_string(move.to.second));
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
            return {max_eval, best_move};
        } else {
            int min_eval = std::numeric_limits<int>::max();
            for (const Move& move : generate_all_possible_moves(board, player)) {
                // Applica la mossa per il giocatore min
                //Logger::info("Applying move MIN: " + to_string(move.from.first) + to_string(move.from.second) + " to " + to_string(move.to.first) + to_string(move.to.second));

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
            return {min_eval, best_move};
        }
    } catch (const exception& e) {
        cerr << "Errore in minimax_alpha_beta: " << e.what() << endl;
        throw;
    }
}