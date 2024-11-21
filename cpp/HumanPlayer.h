#ifndef HUMAN_PLAYER_H
#define HUMAN_PLAYER_H

#include <iostream>
#include "Player.h"
#include "Logger.h"

class HumanPlayer : public Player {
public:
    HumanPlayer(const std::string& name, const std::string& color, const std::string& server_address, int port, int depth);
    void play() override; // Implementazione per il giocatore umano

private:
    bool isValidMoveFormat(const std::string& move); // Funzione di utilità per verificare il formato della mossa
};

#endif // HUMAN_PLAYER_H
