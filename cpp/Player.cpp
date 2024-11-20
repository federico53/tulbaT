#include <iostream>
#include <string>
#include <vector>
#include "GameDriver.h"
#include "Logger.h"

class Player {
protected:
    std::string name;
    std::string color;
    GameDriver driver;

public:
    Player(const std::string& name, const std::string& color, const std::string& server_address, int port)
        : name(name), color(color), driver(GameDriver(color, server_address, port)) {}

    virtual void play() = 0; // Metodo virtuale puro

    virtual ~Player() = default;
};
