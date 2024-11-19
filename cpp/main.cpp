#include <iostream>
#include <string>
#include <stdexcept>
#include <cctype>
#include "Player.h"
#include "Logger.h"

void main() {
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
            server_address = {"localhost", 5800};
        } else {
            server_address = {"localhost", 5801};
        }

        Logger::info("Starting game for player " + player_name + " as " + color + " connecting to " + server_address.first + ":" + std::to_string(server_address.second));

        Player player(player_name, color, server_address);
        player.play();

    } catch (const std::exception &e) {
        Logger::critical("Failed to start the game: " + std::string(e.what()));
        std::cerr << "Failed to start the game: " << e.what() << std::endl;
    }

    return;
}
