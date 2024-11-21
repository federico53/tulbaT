#include "Player.h"

Player::Player(const std::string& name, const std::string& color, const std::string& server_address, int port, int depth)
    : name(name), color(color), driver(GameDriver(color, server_address, port)), depth(depth) {}

// No need to redefine the class here, just implement the member functions