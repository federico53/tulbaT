#include "HumanPlayer.h"

using namespace std;

HumanPlayer::HumanPlayer(const std::string& name, const std::string& color, const std::string& server_address, int port, int depth)
    : Player(name, color, server_address, port, depth) {
    Logger::info("HumanPlayer " + this->name + " initialized as " + this->color);
}

void HumanPlayer::play() {
    try {
        driver.connect(name);
    } catch (const std::exception& e) {
        Logger::critical("Failed to connect: " + std::string(e.what()));
        return;
    }

    while (true) {
        try {
            // Ricevi lo stato di gioco
            Logger::info("Waiting for the game state...");
            auto game_state = driver.receiveGameState();
            auto board = game_state["board"];
            Logger::debug("Received game state: " + game_state.toStyledString());

            // Converti la scacchiera in un formato leggibile
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
            while (!isValidMoveFormat(move)) {
                Logger::error("Invalid move format. Please use algebraic notation (e.g., 'e2e4').");
                std::cout << "Enter your move again (e.g., 'e2e4'): ";
                std::cin >> move;
            }

            // Converti la mossa nel formato richiesto dal server
            Move parsed_move = parseMove(move);

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

bool HumanPlayer::isValidMoveFormat(const std::string& move) {
    return move.length() == 4 &&
            std::isalpha(move[0]) && std::isdigit(move[1]) &&
            std::isalpha(move[2]) && std::isdigit(move[3]);
}
