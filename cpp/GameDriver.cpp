#include "GameDriver.h"
#include <asio.hpp>
#include <json/json.h>
#include <iostream>
#include <sstream>

GameDriver::GameDriver(const std::string& color, const std::string& server_address, int port)
    : color(color), socket(io_context) {
    server_address = asio::ip::tcp::endpoint(asio::ip::address::from_string(server_address), port);
    Logger::info("GameDriver initialized with color " + this->color + " and server " + server_address.address().to_string() + ":" + std::to_string(port));
}

void GameDriver::connect(const std::string& player_name) {
    try {
        socket.connect(server_address);
        Logger::info("Connected to server at " + server_address.address().to_string() + ":" + std::to_string(server_address.port()));

        // Invia la lunghezza del nome del giocatore e il nome
        uint32_t name_length = player_name.size();
        asio::write(socket, asio::buffer(&name_length, sizeof(name_length)));
        asio::write(socket, asio::buffer(player_name));

        Logger::info("Sent player name: " + player_name);
    } catch (std::exception& e) {
        Logger::error("Failed to connect or send player name: " + std::string(e.what()));
        throw;
    }
}

void GameDriver::sendMove(const std::pair<std::pair<int, int>, std::pair<int, int>>& move, const std::string& color) {
    try {
        std::string move_for_server = convertMoveForServer(move, color);

        uint32_t move_length = move_for_server.size();
        asio::write(socket, asio::buffer(&move_length, sizeof(move_length)));
        asio::write(socket, asio::buffer(move_for_server));

        Logger::info("Sent move: " + move_for_server);
    } catch (std::exception& e) {
        Logger::error("Failed to send move: " + std::string(e.what()));
        throw;
    }
}

std::string GameDriver::convertMoveForServer(const std::pair<std::pair<int, int>, std::pair<int, int>>& move, const std::string& color) {
    try {
        std::string from_pos = coordinateToAlgebraic(move.first.first, move.first.second);
        std::string to_pos = coordinateToAlgebraic(move.second.first, move.second.second);

        Json::Value json_move;
        json_move["from"] = from_pos;
        json_move["to"] = to_pos;
        json_move["turn"] = color;

        Json::StreamWriterBuilder writer;
        return Json::writeString(writer, json_move);
    } catch (std::exception& e) {
        Logger::error("Error in convertMoveForServer: " + std::string(e.what()));
        throw;
    }
}

std::string GameDriver::coordinateToAlgebraic(int row, int col) {
    try {
        const std::string columns = "abcdefghi";
        if (col < 0 || col >= columns.size()) throw std::out_of_range("Column index out of range");
        return columns[col] + std::to_string(row + 1);
    } catch (std::exception& e) {
        Logger::error("Error in coordinateToAlgebraic: " + std::string(e.what()));
        throw;
    }
}

Json::Value GameDriver::receiveGameState() {
    try {
        uint32_t len;
        asio::read(socket, asio::buffer(&len, sizeof(len)));

        std::string data = recvall(len);
        Json::CharReaderBuilder reader;
        Json::Value game_state;
        std::istringstream(data) >> game_state;

        return game_state;
    } catch (std::exception& e) {
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
