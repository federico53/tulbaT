#include <iostream>
#include <string>
#include <vector>
#include "GameDriver.h" // Equivalente della classe GameDriver in Python
#include "GameUtils.h"  // Funzioni minimax_alpha_beta, format_board
#include "Logger.h"     // Logger per le informazioni
#include "Stats.h"      // stats_of_the_board

class Player {
private:
    std::string name;
    std::string color;
    GameDriver driver;

public:
    Player(const std::string& name, const std::string& color, const std::string& server_address)
        : name(name), color(color), driver(GameDriver(color, server_address)) {
        Logger::info("Player " + this->name + " initialized as " + this->color);
    }

    void play() {
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
                std::string turn = game_state["turn"];
                std::transform(turn.begin(), turn.end(), turn.begin(), ::tolower);

                Logger::info("Current turn: " + turn);
                Logger::debug("Current board state: \n" + format_board(board));

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
                Logger::debug("Stats of the board: " + stats_of_the_board(board, color));

                // Trova la miglior mossa usando Minimax
                int best_score;
                std::string best_move;
                std::tie(best_score, best_move) = minimax_alpha_beta(board, 1, 2, -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), turn, color);

                Logger::info("Best move for " + color + ": " + best_move + " with score: " + std::to_string(best_score));

                // Invia la mossa al server
                Logger::info("Sending move: " + best_move);
                driver.sendMove(best_move, color);

            } catch (const std::exception& e) {
                Logger::error("An error occurred during the game loop: " + std::string(e.what()));
                break;
            }
        }

        driver.close();
    }
};


