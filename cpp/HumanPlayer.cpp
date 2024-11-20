#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "GameDriver.h" // Classe per la gestione della comunicazione con il server
#include "Logger.h"     // Logger per log di informazioni e errori
#include "GameUtils.h"  // Funzioni ausiliarie per gestire le mosse e il formato della scacchiera

class HumanPlayer {
private:
    std::string name;
    std::string color;
    GameDriver driver;

public:
    HumanPlayer(const std::string& name, const std::string& color, const std::string& server_address, int port)
        : name(name), color(color), driver(GameDriver(color, server_address, port)) {
        Logger::info("HumanPlayer " + this->name + " initialized as " + this->color);
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

                // Converti la scacchiera in un formato leggibile
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

                // Stampa la scacchiera e chiedi all'utente di inserire la mossa
                Logger::info("Your turn! (" + color + ")");
                std::string move;
                std::cout << "Enter your move (e.g., 'e2e4'): ";
                std::cin >> move;

                // Verifica il formato della mossa
                if (!isValidMoveFormat(move)) {
                    Logger::error("Invalid move format. Please use algebraic notation (e.g., 'e2e4').");
                    continue;
                }

                // Converti la mossa nel formato richiesto dal server
                std::pair<std::pair<int, int>, std::pair<int, int>> parsed_move = parseMove(move);

                // Invia la mossa al server
                Logger::info("Sending move: " + move);
                driver.sendMove(parsed_move, color);

            } catch (const std::exception& e) {
                Logger::error("An error occurred during the game loop: " + std::string(e.what()));
                break;
            }
        }

        driver.close();
    }

private:
    bool isValidMoveFormat(const std::string& move) {
        if (move.length() != 4) return false;
        return std::isalpha(move[0]) && std::isdigit(move[1]) &&
               std::isalpha(move[2]) && std::isdigit(move[3]) &&
               move[0] >= 'a' && move[0] <= 'h' &&
               move[2] >= 'a' && move[2] <= 'h' &&
               move[1] >= '1' && move[1] <= '8' &&
               move[3] >= '1' && move[3] <= '8';
    }
};
