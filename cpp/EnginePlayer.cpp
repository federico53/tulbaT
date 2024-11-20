#include <string>
#include <vector>
#include "Player.h"
#include "GameUtils.h"  // Funzioni minimax_alpha_beta, format_board
#include "Stats.h"      // stats_of_the_board

class EnginePlayer : public Player {
public:
    EnginePlayer(const std::string& name, const std::string& color, const std::string& server_address, int port)
        : Player(name, color, server_address, port) {
        Logger::info("EnginePlayer " + this->name + " initialized as " + this->color);
    }

    void play() override {
        try {
            driver.connect(name);
        } catch (const std::exception& e) {
            Logger::critical("Failed to connect: " + std::string(e.what()));
            return;
        }

        while (true) {
            try {
                // Ricevi lo stato di gioco
                auto game_state = driver.receiveGameState();
                auto board = game_state["board"];

                // Convert the board into string
                std::vector<std::vector<std::string>> board_vector;
                for (const auto& row : board) {
                    std::vector<std::string> row_vector;
                    for (const auto& cell : row) {
                        row_vector.push_back(cell.asString());
                    }
                    board_vector.push_back(row_vector);
}
                std::string turn = game_state["turn"].asString();
                std::transform(turn.begin(), turn.end(), turn.begin(), ::tolower);

                Logger::info("Current turn: " + turn);
                Logger::debug("Current board state: \n" + format_board(board_vector));

                // Controlla se il gioco è finito
                if (turn == "whitewin" || turn == "blackwin" || turn == "draw") {
                    Logger::info("Game over! Result: " + turn);
                    break;
                }

                // Aspetta il turno
                if (turn != color) {
                    Logger::info("Waiting for the opponent's turn... (Current turn: " + turn + ")");
                    continue;
                }

                // Stampa le statistiche della scacchiera
                Logger::debug("Stats of the board: " + stats_of_the_board(board_vector, color));

                // Convert the board into integer
                std::vector<std::vector<int>> board_int;
                for (const auto& row : board) {
                    std::vector<int> row_int;
                    for (const auto& cell : row) {
                        row_int.push_back(cell.asInt());
                    }
                    board_int.push_back(row_int);
}
                // Trova la miglior mossa usando Minimax
                int best_score;
                std::string best_move;

                std::pair<int, Move> result = minimax_alpha_beta(board_int, 3, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), turn, color);
                best_score = result.first;
                best_move = result.second;
                
                Logger::info("Best move for " + color + ": " + best_move + " with score: " + std::to_string(best_score));

                // Invia la mossa al server
                Logger::info("Sending move: " + best_move);
                std::pair<std::pair<int, int>, std::pair<int, int>> parsed_move = parseMove(best_move);
                driver.sendMove(parsed_move, color);

            } catch (const std::exception& e) {
                Logger::error("An error occurred during the game loop: " + std::string(e.what()));
                break;
            }
        }

        driver.close();
    }
};
