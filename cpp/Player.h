#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "GameDriver.h" // Classe per gestire la connessione al server di gioco
#include "Logger.h"     // Per la gestione dei log
#include "GameUtils.h"  // Per minimax_alpha_beta e format_board
#include "Stats.h"      // Per stats_of_the_board

class Player {
private:
    std::string name;   // Nome del giocatore
    std::string color;  // Colore del giocatore (es. "white" o "black")
    GameDriver driver;  // Oggetto che gestisce le interazioni con il server

public:
    // Costruttore
    Player(const std::string& name, const std::string& color, const std::string& server_address);

    // Metodo principale per avviare la partita
    void play();
};

#endif // PLAYER_H
