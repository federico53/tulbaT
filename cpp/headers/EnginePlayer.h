#ifndef ENGINE_PLAYER_H
#define ENGINE_PLAYER_H

#include "Player.h"
#include "Logger.h"
#include "MinMax.h"
#include "Stats.h"

class EnginePlayer : public Player {
public:
    EnginePlayer(const std::string& name, const std::string& color, const std::string& server_address, int port, int depth, int cut_size);
    void play() override; // Implementazione per il motore automatico
};

#endif // ENGINE_PLAYER_H
