#include <iostream>
#include <memory>
#include "headers/Logger.h"
#include "headers/EnginePlayer.h"
#include "headers/HumanPlayer.h"

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
        Logger::info("Select the game mode");
        Logger::info("  1 -> Engine player");
        Logger::info("  2 -> Human player");

        int mode = 0;
        std::string input;

        while (true) {
            std::cout << "Enter your choice (1 or 2): ";
            std::getline(std::cin, input);

            try {
                mode = std::stoi(input);
                if (mode == 1 || mode == 2){
                    break;
                }
                Logger::warning("Insert a number between 1 and 2");
            } catch (const std::exception &) {
                Logger::warning("Invalid input. Please enter a valid number.");
            }
        }

        Logger::info("Select the depth of the tree search");

        int depth = 0;
        std::string inputDepth;

        while (true) {
            std::cout << "Enter your choice: ";
            std::getline(std::cin, inputDepth);

            try {
                depth = std::stoi(inputDepth);
                if (depth > 0){
                    break;
                }
                Logger::warning("Insert an odd number greater than 0");
            } catch (const std::exception &) {
                Logger::warning("Invalid input. Please enter a valid number.");
            }
        }

        Logger::info("Select the cut size (0 for no cut)");

        int cut_size = 0;
        std::string inputCutSize;

        while (true) {
            std::cout << "FAKE CHOICE -> Passed 10 and not the parameter" << endl;
            std::cout << "Enter your choice: ";
            std::getline(std::cin, inputCutSize);

            try {
                cut_size = std::stoi(inputCutSize);
                if (cut_size >= 0){
                    break;
                }
                Logger::warning("Insert a number greater or equal to 0");
            } catch (const std::exception &) {
                Logger::warning("Invalid input. Please enter a valid number.");
            }
        }

        std::unique_ptr<Player> player;

        if (mode == 1) {
            player = std::make_unique<EnginePlayer>(player_name, color, server_address.first, server_address.second, depth);
        } 
        else if (mode == 2) {
            player = std::make_unique<HumanPlayer>(player_name, color, server_address.first, server_address.second, depth);
        }

        Logger::info("Game started. Waiting for moves...");
        player->play();

    } catch (const std::exception &e) {
        Logger::critical("Failed to start the game: " + std::string(e.what()));
        std::cerr << "Failed to start the game: " << e.what() << std::endl;
    }

    return 0;
}
