#include <iostream>
#include <memory>
#include "Logger.h"
#include "EnginePlayer.h"
#include "HumanPlayer.h"

using namespace std;

int main() {
    try {
        std::string player_name;
        std::string color;
        std::pair<std::string, int> server_address;

        std::cout << "Enter your player name: ";
        std::getline(std::cin, player_name);

        std::cout << "Choose your color (white/black): ";
        std::getline(std::cin, color);
        for (auto &c : color) c = std::tolower(c);

        if (color != "white" && color != "black") {
            throw std::invalid_argument("You must choose 'white' or 'black' as your color.");
        }

        if (color == "white") {
            server_address = {"127.0.0.1", 5800};
        } else {
            server_address = {"127.0.0.1", 5801};
        }

        Logger::info("Starting game for player " + player_name + " as " + color + 
                     " connecting to " + server_address.first + ":" + std::to_string(server_address.second));

        std::unique_ptr<Player> player = std::make_unique<HumanPlayer>(player_name, color, server_address.first, server_address.second);

        Logger::info("Game started. Waiting for moves...");
        player->play();

    } catch (const std::exception &e) {
        Logger::critical("Failed to start the game: " + std::string(e.what()));
        std::cerr << "Failed to start the game: " << e.what() << std::endl;
    }

    return 0;
}
