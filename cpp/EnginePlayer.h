#ifndef ENGINE_PLAYER_H
#define ENGINE_PLAYER_H

#include "Player.h"

class EnginePlayer : public Player {
public:
    EnginePlayer(const std::string& name, const std::string& color, const std::string& server_address, int port);
    void play() override; // Implementazione per il motore automatico
};

#endif // ENGINE_PLAYER_H
