#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "GameDriver.h"

class Player {
protected:
    std::string name;
    std::string color;
    GameDriver driver;
    int depth;

public:
    Player(const std::string& name, const std::string& color, const std::string& server_address, int port, int depth);
    virtual void play() = 0; // Metodo virtuale puro
    virtual ~Player() = default;
};

#endif // PLAYER_H
