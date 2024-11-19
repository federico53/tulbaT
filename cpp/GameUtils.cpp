#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <sstream>

using namespace std;

// CONSTANTS UTILS

const vector<vector<string>> empty_board = {
    {"EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY"},
    {"EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY"},
    {"EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY"},
    {"EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY"},
    {"EMPTY", "EMPTY", "EMPTY", "EMPTY", "THRONE", "EMPTY", "EMPTY", "EMPTY", "EMPTY"},
    {"EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY"},
    {"EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY"},
    {"EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY"},
    {"EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY"}
};

const vector<pair<int, int>> citadels = {
    {0, 3}, {0, 4}, {0, 5}, {1, 4}, {3, 0}, {3, 1}, {3, 2},
    {4, 0}, {4, 2}, {4, 6}, {4, 8}, {5, 0}, {8, 3}, {8, 5}
};

const vector<pair<int, int>> winning_positions = {
    {0, 1}, {0, 2}, {0, 6}, {0, 7}, {1, 0}, {1, 8}, {2, 0}, {2, 8},
    {6, 0}, {6, 8}, {7, 0}, {7, 8}, {8, 1}, {8, 2}, {8, 6}, {8, 7}
};

// POSITION VALIDATION

bool is_within_bounds(int row, int col) {
    return row >= 0 && row < 9 && col >= 0 && col < 9;
}

// POSITION UTILS

bool is_castle(int row, int col) {
    // Verifica se la posizione è il castello (trono)
    int castle_row = 4, castle_col = 4;  // Il castello si trova al centro della scacchiera
    return (row == castle_row && col == castle_col);
}

bool is_adjacent_to_castle(int row, int col) {
    // Verifica se la posizione è adiacente al castello
    int castle_row = 4, castle_col = 4;  // Il castello si trova nella posizione (4, 4)
    std::vector<std::pair<int, int>> adjacent_positions = {
        {castle_row + 1, castle_col},  // Sotto
        {castle_row - 1, castle_col},  // Sopra
        {castle_row, castle_col + 1},  // Destra
        {castle_row, castle_col - 1}   // Sinistra
    };

    for (const auto& pos : adjacent_positions) {
        if (pos.first == row && pos.second == col) {
            return true;
        }
    }
    return false;
}

bool is_king_captured(const std::vector<std::vector<std::string>>& board) {
    try {
        auto king_pos = stats.get_king_position(board);
        if (king_pos.empty()) {
            return false;  // No king found, so it's not captured
        }

        int king_row = king_pos.first;
        int king_col = king_pos.second;

        std::vector<std::pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};  // Down, Up, Right, Left

        // Case 1: Check if the king is in the castle
        if (is_castle(king_row, king_col)) {
            // King must be surrounded on all four sides by black pieces to be captured
            for (const auto& dir : directions) {
                int new_row = king_row + dir.first;
                int new_col = king_col + dir.second;
                if (is_within_bounds(new_row, new_col, board) && board[new_row][new_col] == "BLACK") {
                    continue;
                }
                return false;  // If any side is not surrounded by black, king is not captured
            }
            return true;  // All sides are surrounded by black pieces
        }

        // Case 2: Check if the king is adjacent to the castle
        else if (is_adjacent_to_castle(king_row, king_col)) {
            // King is captured if surrounded on three sides by black pieces
            int black_count = 0;
            for (const auto& dir : directions) {
                int new_row = king_row + dir.first;
                int new_col = king_col + dir.second;
                if (is_within_bounds(new_row, new_col, board) && board[new_row][new_col] == "BLACK") {
                    black_count++;
                }
            }
            return black_count >= 3;
        }

        // Case 3: General capture condition for non-castle, non-adjacent cells
        else {
            // King must be surrounded on all four sides by black pieces or blocking cells
            for (const auto& dir : directions) {
                int new_row = king_row + dir.first;
                int new_col = king_col + dir.second;
                if (is_within_bounds(new_row, new_col, board) &&
                    (board[new_row][new_col] == "BLACK" || is_blocking_cell(new_row, new_col, board))) {
                    continue;
                }
                return false;  // If any side is not surrounded by black or blocking cells, king is not captured
            }
            return true;  // All sides are surrounded by black pieces or blocking cells
        }
    } catch (const std::exception& e) {
        std::cerr << "Error in is_king_captured: " << e.what() << std::endl;
        throw;
    }
}

bool is_blocking_cell(int row, int col, const std::vector<std::vector<std::string>>& board) {
    // Verifica se la cella è una cella di blocco (come un castello o una cittadella)
    return board[row][col] == "CASTLE" || board[row][col] == "CITADEL";
}


// OPPOSITE TURN

string get_opposite_turn(const string &turn) {
    return (turn == "white") ? "black" : "white";
}

// MOVE VALIDATION

bool is_valid_move(pair<int, int> from, pair<int, int> to, const vector<vector<string>> &board, const string &color) {
    int from_row = from.first, from_col = from.second;
    int to_row = to.first, to_col = to.second;

    // Check if destination is empty
    if (board[to_row][to_col] != "EMPTY") return false;

    // Check if movement is valid (straight line, no obstacles)
    if (from_row == to_row) {
        int step = (to_col > from_col) ? 1 : -1;
        for (int col = from_col + step; col != to_col; col += step) {
            if (board[from_row][col] != "EMPTY") return false;
        }
    } else if (from_col == to_col) {
        int step = (to_row > from_row) ? 1 : -1;
        for (int row = from_row + step; row != to_row; row += step) {
            if (board[row][from_col] != "EMPTY") return false;
        }
    } else {
        return false; // Diagonal moves not allowed
    }

    return true;
}

// MOVE GENERATION

vector<pair<pair<int, int>, pair<int, int>>> generate_all_possible_moves(const vector<vector<string>> &board, const string &color) {
    vector<pair<pair<int, int>, pair<int, int>>> moves;
    vector<pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    for (int row = 0; row < board.size(); ++row) {
        for (int col = 0; col < board[row].size(); ++col) {
            if ((color == "white" && (board[row][col] == "WHITE" || board[row][col] == "KING")) ||
                (color == "black" && board[row][col] == "BLACK")) {
                for (auto &[dr, dc] : directions) {
                    for (int i = 1; i < board.size(); ++i) {
                        int new_row = row + dr * i, new_col = col + dc * i;
                        if (!is_within_bounds(new_row, new_col) || board[new_row][new_col] != "EMPTY") break;

                        pair<int, int> from = {row, col}, to = {new_row, new_col};
                        if (is_valid_move(from, to, board, color)) {
                            moves.push_back({from, to});
                        }
                    }
                }
            }
        }
    }

    return moves;
}

// WHITE HEURISTIC

int heuristic_white(const vector<vector<int>>& board) {
    try {
        // 1. Posizione del re - TODO
        // 2. Posizione del re ai lati - TODO
        
        // 3. Scacco matto al nero
        int points_king_checkmate = 0;
        if (stats::black_checkmate(board)) {
            points_king_checkmate = 100;
        }

        // 4. Differenza tra i pezzi mangiati dal nero e quelli persi dal bianco
        int white_lost = 8 - stats::count_pieces(board, "WHITE");
        int black_eaten = 16 - stats::count_pieces(board, "BLACK");
        int diff_between_white_black = 1600 * (black_eaten - white_lost) * 200;

        // 5. Nero mangiato
        int score_black_eaten = 5000 * black_eaten;

        // 6. Il re non è bloccato nel quadrante - TODO
        int score_king_not_blocked_quadrant = 0;

        // 7. Il nero non può fare scacco matto in una mossa
        int score_king_not_checkmated_infuture = 0;
        if (!stats::black_can_checkmate_in_future(board)) {
            score_king_not_checkmated_infuture = 20000;
        }

        // 8. Il re non è in scacco matto dal nero
        int score_king_not_checkmated = 0;
        if (!stats::white_checkmate(board)) {
            score_king_not_checkmated = 40000;
        }

        // 9. Se il nero non ha vinto
        int white_didnt_win_points = 0;
        if (!is_game_over(board)) {
            white_didnt_win_points = 80000;
        }

        // 10. Se il bianco ha vinto
        int black_won_points = 0;
        if (is_game_over(board)) {
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
}

// BLACK HEURISTIC

int calculate_black_starting_positions_points(const vector<vector<string>>& board) {
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
            if (board[row][col] == "BLACK") {
                points += 3;
            }
        }

        for (const auto& pos : medium_positions) {
            int row = pos.first;
            int col = pos.second;
            if (board[row][col] == "BLACK") {
                points += 2;
            }
        }

        for (const auto& pos : low_positions) {
            int row = pos.first;
            int col = pos.second;
            if (board[row][col] == "BLACK") {
                points += 1;
            }
        }

        return points;
    } catch (const exception& e) {
        cerr << "Error in calculate_black_starting_positions_points: " << e.what() << endl;
        throw;  // Rilancia l'eccezione
    }
}

int heuristic_black(const std::vector<std::vector<std::string>>& board) {
    try {
        // 1. Punti per le posizioni di partenza dei pezzi neri
        int black_starting_positions_points = calculate_black_starting_positions_points(board);

        // 2. Differenza tra i pezzi bianchi mangiati e i pezzi neri persi
        int white_eaten = 8 - stats::count_pieces(board, "WHITE");
        int black_lost = 16 - stats::count_pieces(board, "BLACK");
        int diff_between_white_black = 480 + (white_eaten - black_lost) * 30;

        // 3. Punti per ogni pezzo bianco mangiato
        int points_white_eaten = white_eaten * 750;

        // 4. Se il re non può dare scacco matto in una mossa
        int king_not_checkmate_inonemove = 0;
        if (!stats::king_can_checkmate_in_future(board)) {
            king_not_checkmate_inonemove = 6750;
        }

        // 5. Se il re è in scacco matto
        int king_not_checkmate_points = 0;
        if (stats::white_checkmate(board)) {
            king_not_checkmate_points = 13500;
        }

        // 6. Se il nero non è in scacco matto
        int black_checkmate_points = 0;
        if (!stats::black_checkmate(board)) {
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

int heuristic_evaluation(const vector<vector<int>>& board, const string& turn, const string& player) {
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

int heuristic(const vector<vector<int>>& board, const string& turn) {
    // Funzione euristica per il calcolo del punteggio in base al turno
    if (turn != "white") {
        return heuristic_white(board);
    } else {
        return heuristic_black(board);
    }
}

// MINIMAX ALPHA BETA PRUNING

pair<int, Move> minimax_alpha_beta(vector<vector<int>>& board, int depth, int alpha, int beta, const string& turn, const string& player) {
    try {
        // Caso base: limite di profondità raggiunto o fine del gioco
        if (depth == 0 || is_game_over(board)) {
            return {heuristic_evaluation(board, turn, player), Move()};  // Restituisce punteggio e nessuna mossa usando l'euristica
        }

        Move best_move;
        bool is_max = (turn == player);  // Se il turno è del giocatore, è una fase di massimizzazione

        if (is_max) {
            int max_eval = INT_MIN;
            for (const Move& move : generate_all_possible_moves(board, player)) {
                // Applica la mossa per il giocatore max
                vector<vector<int>> new_board = apply_move(board, move);
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
            int min_eval = INT_MAX;
            for (const Move& move : generate_all_possible_moves(board, player)) {
                // Applica la mossa per il giocatore min
                vector<vector<int>> new_board = apply_move(board, move);
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

// APPLY MOVE

std::vector<std::vector<std::string>> apply_move(
    const std::vector<std::vector<std::string>>& board, 
    const std::pair<std::pair<int, int>, std::pair<int, int>>& move) {

    try {
        // Decomponiamo la mossa
        auto[from_pos, to_pos] = move;
        int from_row = from_pos.first, from_col = from_pos.second;
        int to_row = to_pos.first, to_col = to_pos.second;

        // Creiamo una nuova scacchiera copiando la vecchia
        std::vector<std::vector<std::string>> new_board = board;
        
        // Muoviamo il pezzo sulla nuova scacchiera
        new_board[to_row][to_col] = new_board[from_row][from_col];
        new_board[from_row][from_col] = "EMPTY";

        // Direzioni in cui un pezzo può catturare
        std::vector<std::pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

        // Controlliamo se un pezzo è stato catturato
        for (const auto& [dr, dc] : directions) {
            int check_row = to_row + dr;
            int check_col = to_col + dc;
            int capture_row = to_row + 2*dr;
            int capture_col = to_col + 2*dc;

            // Verifica se la posizione è valida e se c'è un pezzo nemico da catturare
            if (is_within_bounds(capture_row, capture_col, new_board) && 
                std::find(get_enemies(new_board[to_row][to_col]).begin(), get_enemies(new_board[to_row][to_col]).end(), new_board[check_row][check_col]) != get_enemies(new_board[to_row][to_col]).end()) {
                
                // Controlliamo se un pezzo può essere catturato
                if (std::find(get_allies(new_board[to_row][to_col]).begin(), get_allies(new_board[to_row][to_col]).end(), new_board[capture_row][capture_col]) != get_allies(new_board[to_row][to_col]).end()) {
                    new_board[check_row][check_col] = "EMPTY";
                }
            }
        }

        return new_board;

    } catch (const std::exception& e) {
        std::cerr << "Error in apply_move: " << e.what() << std::endl;
        throw; // Rilancia l'eccezione
    }
}

// GAME OVER

std::string is_game_over(const std::vector<std::vector<std::string>>& board) {
    try {
        // 1. Check if the king is in an escape cell (white wins)
        auto king_position = stats.get_king_position(board);
        if (!king_position.empty() && std::find(winning_positions.begin(), winning_positions.end(), king_position) != winning_positions.end()) {
            return "white";  // White player wins
        }
        
        // 2. Check if the king is captured (black wins)
        if (is_king_captured(board)) {
            return "black";  // Black player wins
        }

        // 3. Check if a player has no possible moves (the other player wins)
        if (generate_all_possible_moves(board, "white").empty()) {
            return "black";  // Black player wins
        }
        if (generate_all_possible_moves(board, "black").empty()) {
            return "white";  // White player wins
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error in is_game_over: " << e.what() << std::endl;
        throw;
    }
    return "";  // No winner yet
}

// BOARD REPRESENTATION

std::string format_board(const std::vector<std::vector<std::string>>& board) {
    // Rappresentazione simbolica della scacchiera
    std::map<std::string, char> symbols = {
        {"WHITE", 'W'},
        {"BLACK", 'B'},
        {"KING", 'K'},
        {"THRONE", 'T'},
        {"EMPTY", ' '}
    };

    // Etichette delle colonne
    std::string column_labels = "A B C D E F G H I";
    std::string board_str = "    ";
    
    // Aggiungi le etichette delle colonne
    std::istringstream col_stream(column_labels);
    std::string col;
    while (col_stream >> col) {
        board_str += col + "   ";
    }
    board_str += "\n";
    board_str += "  +" + std::string(4 * 9, '-') + "+\n";

    // Aggiungi le righe della scacchiera
    for (int i = 0; i < board.size(); ++i) {
        board_str += std::to_string(i + 1) + " | ";
        for (int j = 0; j < board[i].size(); ++j) {
            board_str += symbols[board[i][j]] + " | ";
        }
        board_str += "\n";
        board_str += "  +" + std::string(4 * 9, '-') + "+\n";
    }

    return board_str;
}




