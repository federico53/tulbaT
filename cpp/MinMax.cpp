#include "headers/MinMax.h"

using namespace std;


// WHITE HEURISTIC
// int heuristic_white(const vector<vector<char>>& board) {
//     try {
//         // 1. Posizione del re - TODO
//         // 2. Posizione del re ai lati - TODO
        
//         // 3. Scacco matto al nero
//         int points_king_checkmate = 0;
//         if (black_checkmate(board)) {
//             points_king_checkmate = 100;
//         }

//         //cout << "points_king_checkmate: done" << endl;

//         // 4. Differenza tra i pezzi mangiati dal nero e quelli persi dal bianco
//         int white_lost = 8 - count_pieces(board, 'W');
//         int black_eaten = 16 - count_pieces(board, 'B');
//         int diff_between_white_black = 1600 + (black_eaten - white_lost) * 200;

//         //cout << "diff_between_white_black: done" << endl;

//         // 5. Nero mangiato
//         int score_black_eaten = 5000 * black_eaten;

//         //cout << "score_black_eaten: done" << endl;

//         // 6. Il re non è bloccato nel quadrante - TODO
//         int score_king_not_blocked_quadrant = 0;

//         //cout << "score_king_not_blocked_quadrant: done" << endl;

//         // // 7. Il nero non può fare scacco matto in una mossa
//         // int score_king_not_checkmated_infuture = 0;
//         // if (!black_can_checkmate_in_future(board)) {
//         //     score_king_not_checkmated_infuture = 20000;
//         // }

//         //cout << "score_king_not_checkmated_infuture: done" << endl;

//         // 8. Il re non è in scacco matto dal nero
//         int score_king_not_checkmated = 0;
//         if (!white_checkmate(board)) {
//             score_king_not_checkmated = 40000;
//         }

//         //cout << "score_king_not_checkmated: done" << endl;

//         // 9. Se il nero non ha vinto
//         int white_didnt_win_points = 0;
//         if (is_game_over(board) != "black") {
//             white_didnt_win_points = 80000;
//         }

//         //cout << "white_didnt_win_points: done" << endl;

//         // 10. Se il bianco ha vinto
//         int white_won_points = 0;
//         if (is_game_over(board) == "white") {
//             white_won_points = 160000/0.16; // 160000/0.16 = 1000000
//             return white_won_points;
//         }
//         //cout << "white_won_points: done" << endl;

        
//         // Calcolo finale del punteggio
//         return  (points_king_checkmate + 
//                 diff_between_white_black + 
//                 score_black_eaten + 
//                 score_king_not_blocked_quadrant + 
//                 //score_king_not_checkmated_infuture + 
//                 score_king_not_checkmated + 
//                 white_didnt_win_points)/0.16;

//     } catch (const exception& e) {
//         cerr << "Error in heuristic_white: " << e.what() << endl;
//         return 0;  // Gestione dell'errore
//     }
// } 

int heuristic_white(const vector<vector<char>>& board) {
    try {

        // 0. Se il bianco ha perso
        int white_lose_points = 0;
        if (is_game_over(board) == "black") {
            white_lose_points = 0; 
            return white_lose_points;
        }

        //cout << "0." <<endl;

        // 1. Posizionamento del re
        // range (1-5)
        int score_king_winning_direction = king_winning_direction_score(board) + 1;  

        int white_lost = 8 - count_pieces(board, 'W');
        int black_eaten = 16 - count_pieces(board, 'B');

        //cout << "1." <<endl;

        // 2. Pezzi neri mangiati
        // range (0-96)
        int score_black_eaten = black_eaten * 6;

        //cout << "2." <<endl;

        // 3. Differenza tra i pezzi mangiati dal nero e quelli persi dal bianco
        // range (0-1632)
        int diff_between_white_black = 816 + (black_eaten - white_lost) * 102;

        //cout << "3." <<endl;

        // 4. Il re non è bloccato nel quadrante
        // range (0-1734)
        int score_king_not_blocked_quadrant = king_block_in_quadrant(board) ? 0 : 1734;

        //cout << "4." <<endl;

        auto [free_sides, blocked_sides, black_blockers, white_blockers, castle_blockers] = king_free_sides(board);
        
        // 5. Pezzi neri che circondano il re
        // range (0-13872)
        int score_black_blockers = (4-black_blockers) * 3468;

        //cout << "5." <<endl;

        // 6. Il re puo scappare
        // range(0-17340)
        int score_king_can_escape = black_checkmate(board) ? 17340 : 0;

        //cout << "6." <<endl;

        // 7. Il re puo scappare su due lati
        // range(0-34680)  
        // TO-DO
        int score_king_can_escape_two_sides = double_escape_for_the_king(board) ? 34680 : 0;

        //cout << "7." <<endl;

        // 8. Il re non puo essere catturato
        // range(0-69360)
        int score_king_cant_be_captured = white_checkmate(board) ? 0 : 69360;

        //cout << "8." <<endl;

        // Maximum sum of all the scores = 138719

        // 9. Se il bianco ha vinto
        int white_win_points = 0;
        if (is_game_over(board) == "white") {
            white_win_points = 1000000; 
            return white_win_points;
        }

        //cout << "9." <<endl;
        
        // Calcolo finale del punteggio
        return  (score_king_winning_direction + 
                score_black_eaten + 
                diff_between_white_black + 
                score_king_not_blocked_quadrant + 
                score_black_blockers + 
                score_king_can_escape + 
                //score_king_can_escape_two_sides +
                score_king_cant_be_captured)/0.138720;

    } catch (const exception& e) {
        cerr << "Error in heuristic_white: " << e.what() << endl;
        return 0;  // Gestione dell'errore
    }
} 

// int heuristic_white(const vector<vector<char>>& board) {
//     try {

//         // 0. Se il bianco ha perso
//         int white_lose_points = 0;
//         if (is_game_over(board) == "black") {
//             white_lose_points = 0; 
//             return white_lose_points;
//         }

//         //cout << "0." <<endl;

//         // 1. Posizionamento del re
//         // range (1-5)
//         int score_king_winning_direction = king_winning_direction_score(board) + 1;  

//         int white_lost = 8 - count_pieces(board, 'W');
//         int black_eaten = 16 - count_pieces(board, 'B');

//         //cout << "1." <<endl;

//         // 2. Pezzi neri mangiati
//         // range (0-96)
//         int score_black_eaten = black_eaten * 6;

//         //cout << "2." <<endl;

//         // 3. Differenza tra i pezzi mangiati dal nero e quelli persi dal bianco
//         // range (0-1632)
//         int diff_between_white_black = 816 + (black_eaten - white_lost) * 102;

//         //cout << "3." <<endl;

//         // 4. Il re non è bloccato nel quadrante
//         // range (0-1734)
//         int score_king_not_blocked_quadrant = king_block_in_quadrant(board) ? 0 : 1734;

//         //cout << "4." <<endl;

//         auto [free_sides, blocked_sides, black_blockers, white_blockers, castle_blockers] = king_free_sides(board);
        
//         // 5. Pezzi neri che circondano il re
//         // range (0-13872)
//         int score_black_blockers = (4-black_blockers) * 3468;

//         //cout << "5." <<endl;

//         // 6. Il re puo scappare
//         // range(0-17340)
//         int score_king_can_escape = black_checkmate(board) ? 17340 : 0;

//         //cout << "6." <<endl;

//         // 7. Il re puo scappare su due lati
//         // range(0-34680)  
//         // TO-DO
//         int score_king_can_escape_two_sides = double_escape_for_the_king(board) ? 34680 : 0;

//         //cout << "7." <<endl;

//         // 8. Il re non puo essere catturato
//         // range(0-69360)
//         int score_king_cant_be_captured = white_checkmate(board) ? 0 : 69360;

//         //cout << "8." <<endl;

//         // Maximum sum of all the scores = 138719

//         // 9. Se il bianco ha vinto
//         int white_win_points = 0;
//         if (is_game_over(board) == "white") {
//             white_win_points = 10000000; //Add a 0 for the update
//             return white_win_points;
//         }

//         //cout << "9." <<endl;
        
//         // Calcolo finale del punteggio
//         return  (score_king_winning_direction + 
//                 score_black_eaten + 
//                 diff_between_white_black + 
//                 score_king_not_blocked_quadrant + 
//                 score_black_blockers + 
//                 score_king_can_escape + 
//                 //score_king_can_escape_two_sides +
//                 score_king_cant_be_captured)/0.0138720; //Add a 0 for the update

//     } catch (const exception& e) {
//         cerr << "Error in heuristic_white: " << e.what() << endl;
//         return 0;  // Gestione dell'errore
//     }
// } 

// int heuristic_white(const vector<vector<char>>& board) {

//     int sum = 0;

//     if (is_game_over(board) == "white") {
//         sum += 1000000;
//     }

//     if (is_game_over(board) == "black") {
//         sum -= 2000000;
//     }
    
//     int black_eaten = 16 - count_pieces(board, 'B');
//     sum += black_eaten * 500;

//     int points_king_checkmate = 0;
//         if (black_checkmate(board)) {
//             points_king_checkmate = 1000;
//         }
//     sum += points_king_checkmate;


//     return sum;
    
// }



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

// int heuristic_black(const std::vector<std::vector<char>>& board){
//     try{

//         int score = 0;

//         if (is_game_over(board) == "black") {
//             return 1000000;
//         }

//         if (is_game_over(board) == "white") {
//             return -10000000;
//         }

//         // Points for number of pieces
//         score += 8 - count_pieces(board, 'W') * 140;
//         score -= 16 - count_pieces(board, 'B') * 95;

//         // Points for the starting positions of the black pieces
//         score -= count_black_starting_position(board) * 15;

//         // Points for white risk checkmate
//         if (white_checkmate(board)) {
//             score += 1600;
//         }

//         // Points for black risk checkmate
//         if (black_checkmate(board)) {
//             score -= 3000;
//         }

//         return score;
//     } catch (const std::exception& e) {
//         std::cerr << "Error in heuristic_black: " << e.what() << std::endl;
//         throw; // Rilancia l'eccezione
//     }
// }

// int heuristic_black(const std::vector<std::vector<char>>& board) {
//     try {
//         //Add a print between each function to debug

//         // 1. Punti per le posizioni di partenza dei pezzi neri
//         int black_starting_positions_points = calculate_black_starting_positions_points(board);

//         //cout << "black_starting_positions_points: done" << endl;
        
//         // 2. Differenza tra i pezzi bianchi mangiati e i pezzi neri persi
//         int white_eaten = 8 - count_pieces(board, 'W');
//         int black_lost = 16 - count_pieces(board, 'B');
//         int diff_between_white_black = 480 + (white_eaten - black_lost) * 30;

//         //cout << "diff_between_white_black: done" << endl;
        
//         // 3. Punti per ogni pezzo bianco mangiato
//         int points_white_eaten = white_eaten * 750;

//         //cout << "points_white_eaten: done" << endl;
        
//         // 4. Se il re non può dare scacco matto in una mossa
//         int king_not_checkmate_inonemove = 0;
//         if (!king_can_checkmate_in_future(board)) {
//             king_not_checkmate_inonemove = 6750;
//         }

//         //cout << "king_not_checkmate_inonemove: done" << endl;
        
//         // 5. Se il re è in scacco matto
//         int king_not_checkmate_points = 0;
//         if (white_checkmate(board)) {
//             king_not_checkmate_points = 13500;
//         }

//         //cout << "king_not_checkmate_points: done" << endl;
        
//         // 6. Se il nero non è in scacco matto
//         int black_checkmate_points = 0;
//         if (!black_checkmate(board)) {
//             black_checkmate_points = 27000;
//         }

//         //cout << "black_checkmate_points: done" << endl;
        
//         // 7. Se il bianco non ha vinto
//         int white_didnt_win_points = 0;
//         if (is_game_over(board) != "white") {
//             white_didnt_win_points = 54000;
//         }

//         //cout << "white_didnt_win_points: done" << endl;
        
//         // 8. Se il nero ha vinto
//         int black_won_points = 0;
//         if (is_game_over(board) == "black") {

//             black_won_points = 108000/0.108; // 108000/0.108 = 1000000
//             return black_won_points;
//         }
//         //cout << "black_won_points: done" << endl;

//         return  (black_starting_positions_points + 
//                 diff_between_white_black + 
//                 points_white_eaten + 
//                 king_not_checkmate_inonemove + 
//                 king_not_checkmate_points + 
//                 black_checkmate_points + 
//                 white_didnt_win_points)/0.108;

//     } catch (const std::exception& e) {
//         std::cerr << "Error in heuristic_black: " << e.what() << std::endl;
//         throw; // Rilancia l'eccezione
//     }
// }

int heuristic_black(const std::vector<std::vector<char>>& board) {
    try {
        
        // 0. Se il bianco non ha vinto
        
        if (is_game_over(board) == "white") {
            int black_lose_points = 0;
            return black_lose_points;
        }

        // 1. Punti per le posizioni di partenza dei pezzi neri
        int black_starting_positions_points = calculate_black_starting_positions_points(board);

        //cout << "black_starting_positions_points: done" << endl;
        
        // 2. Differenza tra i pezzi bianchi mangiati e i pezzi neri persi
        int white_eaten = 8 - count_pieces(board, 'W');
        int black_lost = 16 - count_pieces(board, 'B');
        int diff_between_white_black = 480 + (white_eaten - black_lost) * 30;

        //cout << "diff_between_white_black: done" << endl;
        
        // 3. Punti per ogni pezzo bianco mangiato
        int points_white_eaten = white_eaten * 750;

        //cout << "points_white_eaten: done" << endl;
        
        // 4. Se il re non può dare scacco matto in una mossa
        int king_not_checkmate_inonemove = 0;
        if (!king_can_checkmate_in_future(board)) {
            king_not_checkmate_inonemove = 6750;
        }

        //cout << "king_not_checkmate_inonemove: done" << endl;
        
        // 5. Se il re è in scacco matto
        int king_not_checkmate_points = 0;
        if (white_checkmate(board)) {
            king_not_checkmate_points = 13500;
        }

        //cout << "king_not_checkmate_points: done" << endl;
        
        // 6. Se il nero non è in scacco matto
        int black_checkmate_points = 0;
        if (!black_checkmate(board)) {
            black_checkmate_points = 27000;
        }

        // 7. Se il re ha due vie di uscita
        int double_escape = double_escape_for_the_king(board) ? 0 : 54000;

        

        //cout << "white_didnt_win_points: done" << endl;
        
        // 8. Se il nero ha vinto
        int black_won_points = 0;
        if (is_game_over(board) == "black") {

            black_won_points = 108000/0.108; // 108000/0.108 = 1000000
            return black_won_points;
        }
        //cout << "black_won_points: done" << endl;

        return  (black_starting_positions_points + 
                diff_between_white_black + 
                points_white_eaten + 
                king_not_checkmate_inonemove + 
                king_not_checkmate_points + 
                black_checkmate_points + 
                double_escape)/0.108;

    } catch (const std::exception& e) {
        std::cerr << "Error in heuristic_black: " << e.what() << std::endl;
        throw; // Rilancia l'eccezione
    }
}

// int heuristic_black(const std::vector<std::vector<char>>& board) {
//     try {
        
//         // 0. Se il bianco non ha vinto
        
//         if (is_game_over(board) == "white") {
//             int black_lose_points = 0;
//             return black_lose_points;
//         }

//         // 1. Punti per le posizioni di partenza dei pezzi neri
//         int black_starting_positions_points = calculate_black_starting_positions_points(board);

//         //cout << "black_starting_positions_points: done" << endl;

//         // 2. Mosse che può fare il bianco
//         int score_number_of_white_moves = (100 - generate_all_possible_moves(board, 'W').size())*30;
        
//         // 3. Differenza tra i pezzi bianchi mangiati e i pezzi neri persi
//         int white_eaten = 8 - count_pieces(board, 'W');
//         int black_lost = 16 - count_pieces(board, 'B');
//         int diff_between_white_black = 48480 + (white_eaten - black_lost) * 3030;

//         //cout << "diff_between_white_black: done" << endl;
        
//         // 4. Punti per ogni pezzo bianco mangiato
//         int points_white_eaten = white_eaten * 75750;

//         //cout << "points_white_eaten: done" << endl;
        
//         // 5. Se il re non può dare scacco matto in una mossa
//         int king_not_checkmate_inonemove = 0;
//         if (!king_can_checkmate_in_future(board)) {
//             king_not_checkmate_inonemove = 151500;
//         }

//         //cout << "king_not_checkmate_inonemove: done" << endl;
        
//         // 6. Se il re è in scacco matto
//         int king_not_checkmate_points = 0;
//         if (white_checkmate(board)) {
//             king_not_checkmate_points = 303000;
//         }

//         //cout << "king_not_checkmate_points: done" << endl;
        
//         // 7. Se il nero non è in scacco matto
//         int black_checkmate_points = 0;
//         if (!black_checkmate(board)) {
//             black_checkmate_points = 606000;
//         }

//         // 8. Se il re ha due vie di uscita
//         int double_escape = double_escape_for_the_king(board) ? 0 : 1212000;

        
//         //cout << "white_didnt_win_points: done" << endl;
        
//         // 9. Se il nero ha vinto
//         int black_won_points = 0;
//         if (is_game_over(board) == "black") {

//             black_won_points = 2424000/0.2424; // 2424000/0.2424 = 10_000_000
//             return black_won_points;
//         }
//         //cout << "black_won_points: done" << endl;

//         return  (black_starting_positions_points + 
//                 score_number_of_white_moves +
//                 diff_between_white_black + 
//                 points_white_eaten + 
//                 king_not_checkmate_inonemove + 
//                 king_not_checkmate_points + 
//                 black_checkmate_points + 
//                 double_escape)/0.2424; // Update

//     } catch (const std::exception& e) {
//         std::cerr << "Error in heuristic_black: " << e.what() << std::endl;
//         throw; // Rilancia l'eccezione
//     }
// }

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
            std::vector<Move> generated_moves= generate_all_possible_moves(board, player);
            for (const Move& move : generated_moves) {
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
            std::vector<Move> generated_moves= generate_all_possible_moves(board, player);
            for (const Move& move : generated_moves) {
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

pair<int, Move> minimax_alpha_beta_fast(const vector<vector<char>>& board, int depth, int alpha, int beta, const char& turn, const char& player, int cut_size) {
    try {
        // Caso base: limite di profondità raggiunto o fine del gioco
        if (depth == 0 || is_game_over(board) != "") {
            return {heuristic_evaluation(board, turn, player), Move()};  // Restituisce punteggio e nessuna mossa usando l'euristica
        }

        Move best_move;
        bool is_max = (turn == player);  // Se il turno è del giocatore, è una fase di massimizzazione

        // Generazione delle mosse
        std::vector<Move> generated_moves = generate_all_possible_moves(board, turn);

        // Seleziona le migliori mosse basate sull'euristica
        std::vector<std::pair<int, Move>> evaluated_moves; // Lista di valutazioni e mosse
        for (const Move& move : generated_moves) {
            vector<vector<char>> new_board = apply_move(board, move); // Applica la mossa per calcolare l'euristica
            int score = heuristic_evaluation(new_board, turn, player); // Valuta il punteggio
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