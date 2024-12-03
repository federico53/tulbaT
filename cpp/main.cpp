#include <iostream>
#include <memory>
#include "headers/Logger.h"
#include "headers/EnginePlayer.h"
#include "headers/HumanPlayer.h"

using namespace std;

int main(int argc, char *argv[]) {
    try {
        // ********** CHECK FOR THE CORRECT NUMBER OF PARAMETERS *********
        if(argc!=4 && argc!=5 && argc!=6){
            throw std::invalid_argument("You must pass as parameters:\n\t-> the color (\"WHITE\",\"BLACK\")\n\t-> the time (an int)\n\t-> the ip address of the host (\"127.0.0.1\")");
            return -1;
        }

        // ********************* PLAYER NAME FIXED ***********************
        std::string player_name="tulbaT";

        // **************************** COLOR ****************************
        std::string color;
        if(std::string(argv[1])!="WHITE" && std::string(argv[1])!="BLACK"){
            throw std::invalid_argument("You must choose 'WHITE' or 'BLACK' as your color while you have inserted: " + std::string(argv[1]));

            return -1;
        }else{
            color=argv[1];
            for (auto &c : color) c = std::tolower(c);
        }

        // **************************** TIME ****************************
        int time=std::stoi(argv[2]);

        // **************************** HOST ****************************
        std::string host=argv[3];
        std::pair<std::string, int> server_address;


        //******************* COMPOSING SERVER ADDRESS ******************
        if (color == "white") {
            //server_address = {"151.67.250.105", 5800};
            server_address = {host, 5800};
        } else {
            //server_address = {"151.67.250.105", 5801};
            server_address = {host, 5801};
        }
        Logger::info("Starting game for player " + player_name + " as " + color + 
                     " connecting to " + server_address.first + ":" + std::to_string(server_address.second));
        
        // ******************** GAME MODE SELECTION *********************
        // Logger::info("Select the game mode");
        // Logger::info("  1 -> Engine player");
        // Logger::info("  2 -> Human player");

        // int mode = 0;
        // std::string input;

        // while (true) {
        //     std::cout << "Enter your choice (1 or 2): ";
        //     std::getline(std::cin, input);

        //     try {
        //         mode = std::stoi(input);
        //         if (mode == 1 || mode == 2){
        //             break;
        //         }
        //         Logger::warning("Insert a number between 1 and 2");
        //     } catch (const std::exception &) {
        //         Logger::warning("Invalid input. Please enter a valid number.");
        //     }
        // }

        // ******************** DEPTH SIZE SELECTION *********************
        int depth = 4;
        //we decide the depth in accordance to the time, which is between 5 seconds andn 60 seconds
        // if(time<=5)
        //     depth=4;
        // else if(time<=15)
        //     depth=5;
        // else if(time<=60)
        //     depth=6;
        if(argc>=5){ // if the depth is passed as parameter
            depth = std::stoi(argv[4]);
        }else{ // if the depth is not passed as parameter, the user can choose it from the terminal
            Logger::info("Select the depth of the tree search");
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
        }

        // ********************* CUT SIZE SELECTION **********************
        int cut_size;
        if(argc>=6){ // if the cut_size is passed as parameter
            cut_size = std::stoi(argv[5]);
        }else{ // if the cut_size is not passed as parameter, it is fixed = 10
            cut_size=10;
        }

        std::unique_ptr<Player> player=std::make_unique<EnginePlayer>(player_name, color, server_address.first, server_address.second, depth, cut_size, time);;

        // **************** PLAYER INSTANTIATION WITH MODE ****************
        // if (mode == 1) {
        //     player = std::make_unique<EnginePlayer>(player_name, color, server_address.first, server_address.second, depth, cut_size);
        // } 
        // else if (mode == 2) {
        //     player = std::make_unique<HumanPlayer>(player_name, color, server_address.first, server_address.second, depth, cut_size);
        // }

        Logger::info("Game started. Waiting for moves...");
        player->play();

    } catch (const std::exception &e) {
        Logger::critical("Failed to start the game: " + std::string(e.what()));
        std::cerr << "Failed to start the game: " << e.what() << std::endl;
    }

    return 0;
}