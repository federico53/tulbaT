#ifndef HUMAN_PLAYER_H
#define HUMAN_PLAYER_H

#include "Player.h"

class HumanPlayer : public Player {
public:
    HumanPlayer(const std::string& name, const std::string& color, const std::string& server_address, int port);
    void play() override; // Implementazione per il giocatore umano

private:
    bool isValidMoveFormat(Move& move); // Funzione di utilità per verificare il formato della mossa
};

#endif // HUMAN_PLAYER_H
