#include "headers/Player.h"

Player::Player(const std::string& name, const std::string& color, const std::string& server_address, int port, int depth, int cut_size)
    : name(name), color(color), driver(GameDriver(color, server_address, port)), depth(depth), cut_size(cut_size) {}

// No need to redefine the class here, just implement the member functions