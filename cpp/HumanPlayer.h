#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include <string>
#include "GameDriver.h" // Per la gestione della comunicazione con il server

class HumanPlayer {
private:
    std::string name;     // Nome del giocatore
    std::string color;    // Colore del giocatore (white/black)
    GameDriver driver;    // Oggetto per la comunicazione con il server

    // Funzione per verificare se il formato della mossa è valido
    bool isValidMoveFormat(const std::string& move);

public:
    // Costruttore
    HumanPlayer(const std::string& name, const std::string& color, const std::string& server_address, int port);

    // Funzione principale per gestire il gioco
    void play();
};

#endif // HUMANPLAYER_H
