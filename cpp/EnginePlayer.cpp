#include "headers/EnginePlayer.h"

EnginePlayer::EnginePlayer(const std::string& name, const std::string& color, const std::string& server_address, int port, int depth, int cut_size, int time)
    : Player(name, color, server_address, port, depth, cut_size, time) {
    Logger::info("EnginePlayer " + this->name + " initialized as " + this->color);
}

void EnginePlayer::play(){
    try {
        driver.connect(name);
    } catch (const std::exception& e) {
        Logger::critical("Failed to connect: " + std::string(e.what()));
        return;
    }

    while (true) {
        try {
            // Receving the game state
            auto game_state = driver.receiveGameState();
            auto board = game_state["board"];

            // Convert the board into chars
            std::vector<std::vector<char>> board_vector;
            for (const auto& row : board) {
                std::vector<char> row_vector;
                for (const auto& cell : row) {
                    row_vector.push_back(cell.asString()[0]);
                }
                board_vector.push_back(row_vector);
            }
            std::string turn = game_state["turn"].asString();
            std::transform(turn.begin(), turn.end(), turn.begin(), ::tolower);

            Logger::info("Current turn: " + turn);
            Logger::debug("Current board state: \n" + format_board(board_vector));

            // Checking if the game is over
            if (turn == "whitewin" || turn == "blackwin" || turn == "draw") {
                Logger::info("Game over! Result: " + turn);
                break;
            }

            // Waiting for the opponent's turn
            if (turn != color) {
                Logger::info("Waiting for the opponent's turn... (Current turn: " + turn + ")");
                continue;
            }

            // Printing board stats
            //Logger::debug("Stats of the board: " + stats_of_the_board(board_vector, color[0]));

            // Finding the best move with MinMax algorithm
            int best_score;
            Move best_move;

            // CALLING THE MINIMAX FUNCTION
            // std::pair<int, Move> result = minimax_alpha_beta_fast(board_vector, depth, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), std::toupper(turn[0]), std::toupper(color[0]), cut_size);
            std::pair<int, Move> result = run_minimax_with_threads(board_vector, depth, std::toupper(turn[0]), std::toupper(color[0]), cut_size, time);
            // std::pair<int, Move> result = minimax_alpha_beta(board_vector, depth, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), std::toupper(turn[0]), std::toupper(color[0]));

            best_score = result.first;
            best_move = result.second;

            Logger::info("Best move for " + color + ": " + driver.coordinateToAlgebraic(best_move.from.first, best_move.from.second) + " to " + driver.coordinateToAlgebraic(best_move.to.first, best_move.to.second) + " with score: " + std::to_string(best_score));

            // Sending the move to the server
            Logger::info("Sending move: " + driver.coordinateToAlgebraic(best_move.from.first, best_move.from.second) + " to " + driver.coordinateToAlgebraic(best_move.to.first, best_move.to.second));
            driver.sendMove(best_move, color);

        } catch (const std::exception& e) {
            Logger::error("An error occurred during the game loop: " + std::string(e.what()));
            break;
        }
    }

    driver.close();
}