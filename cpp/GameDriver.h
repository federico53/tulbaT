#ifndef GAMEDRIVER_H
#define GAMEDRIVER_H

#include <string>
#include <asio.hpp>   // Per la gestione del socket
#include <jsoncpp/json/json.h> // Per la gestione dei dati JSON
#include "Logger.h"    // Logger per il logging
#include "GameUtils.h" //per la move

class GameDriver {
private:
    std::string color;                      // Colore del giocatore
    asio::io_context io_context;           // Contesto per l'operazione del socket
    asio::ip::tcp::socket socket;          // Socket TCP per comunicare con il server
    asio::ip::tcp::endpoint server_address;// Endpoint del server

public:
    // Costruttore
    GameDriver(const std::string& color, const std::string& server_address, int port);

    // Connessione al server
    void connect(const std::string& player_name);

    // Invio di una mossa al server
    void sendMove(Move& move, const std::string& color);

    // Ricezione dello stato di gioco
    Json::Value receiveGameState();

    // Conversione delle coordinate della mossa per il server
    std::string convertMoveForServer(Move& move, const std::string& color);

    // Conversione di coordinate da (riga, colonna) in notazione algebrica
    std::string coordinateToAlgebraic(int row, int col);

    // Chiude la connessione
    void close();

private:
    // Lettura completa di `n` byte dal socket
    std::string recvall(size_t n);
};

#endif // GAMEDRIVER_H
