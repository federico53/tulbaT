#include "GameDriver.h"

GameDriver::GameDriver(const std::string& color, const std::string& server_address_str, int port)
    : color(color), socket(io_context) {
    server_address = asio::ip::tcp::endpoint(asio::ip::address::from_string(server_address_str), port);
    Logger::info("GameDriver initialized with color " + this->color + " and server " + server_address.address().to_string() + ":" + std::to_string(port));
}

void GameDriver::connect(const std::string& player_name) {
    try {
        // Connessione al server
        socket.connect(server_address);
        Logger::info("Connected to server at " + server_address.address().to_string() + ":" + std::to_string(server_address.port()));

        // Calcola la lunghezza della stringa del nome
        uint32_t name_length = player_name.size();

        // Invia la lunghezza del nome (4 byte, big-endian)
        uint32_t name_length_be = htonl(name_length); // Converti in big-endian
        asio::write(socket, asio::buffer(&name_length_be, sizeof(name_length_be)));

        // Invia la stringa del nome
        asio::write(socket, asio::buffer(player_name));

        Logger::info("Sent player name: " + player_name);
    } catch (const std::exception& e) {
        Logger::error("Failed to connect or send player name: " + std::string(e.what()));
        throw;
    }
}


void GameDriver::sendMove(Move& move, const std::string& color) {
    try {
        // Convert move to JSON format for the server
        std::string move_for_server = convertMoveForServer(move, color);

        // Prepend the length of the move string
        uint32_t move_length = htonl(move_for_server.size()); // Use network byte order
        asio::write(socket, asio::buffer(&move_length, sizeof(move_length)));

        // Send the move string
        asio::write(socket, asio::buffer(move_for_server));

        Logger::info("Sent move: " + move_for_server);
    } catch (std::exception& e) {
        Logger::error("Failed to send move: " + std::string(e.what()));
        throw;
    }
}


std::string GameDriver::convertMoveForServer(Move& move, const std::string& color) {
    try {
        std::string from_pos = coordinateToAlgebraic(move.from.first, move.from.second);
        std::string to_pos = coordinateToAlgebraic(move.to.first, move.to.second);

        Json::Value json_move;
        json_move["from"] = from_pos;
        json_move["to"] = to_pos;
        if (color == "white") {
            json_move["turn"] = "WHITE";
        } else {
            json_move["turn"] = "BLACK";
        }

        Json::StreamWriterBuilder writer;
        return Json::writeString(writer, json_move);
    } catch (std::exception& e) {
        Logger::error("Error in convertMoveForServer: " + std::string(e.what()));
        throw;
    }
}

std::string GameDriver::coordinateToAlgebraic(int row, int col) {
    try {
        const std::string columns = "ABCDEFGHI";
        if (col < 0 || col >= columns.size()) throw std::out_of_range("Column index out of range");
        return columns[col] + std::to_string(row + 1);
    } catch (std::exception& e) {
        Logger::error("Error in coordinateToAlgebraic: " + std::string(e.what()));
        throw;
    }
}

Json::Value GameDriver::receiveGameState() {
    try {
        // Read the 4-byte length of the incoming message
        uint32_t len;
        asio::read(socket, asio::buffer(&len, sizeof(len)));
        len = ntohl(len);  // Convert network byte order to host byte order

        // Receive the JSON string
        std::string data = recvall(len);
        //Logger::info("Received raw data: " + data);

        // Parse the JSON string
        Json::CharReaderBuilder reader;
        Json::Value game_state;
        std::string errs;
        std::istringstream stream(data);
        if (!Json::parseFromStream(reader, stream, &game_state, &errs)) {
            Logger::error("Failed to parse game state: " + errs);
            throw std::runtime_error("Invalid JSON received");
        }

        return game_state;
    } catch (const std::exception &e) {
        Logger::error("Failed to receive game state: " + std::string(e.what()));
        throw;
    }
}


std::string GameDriver::recvall(size_t n) {
    try {
        std::string data;
        data.resize(n);

        asio::read(socket, asio::buffer(&data[0], n));
        return data;
    } catch (std::exception& e) {
        Logger::warning("Socket connection closed unexpectedly.");
        throw;
    }
}

void GameDriver::close() {
    socket.close();
    Logger::info("Socket connection closed.");
}
