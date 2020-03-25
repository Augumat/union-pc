#pragma once
#include <string>
#include <vector>

#include "Connection.hpp"

class Server {
public:
    Server(int port);
    void broadcastMessage(const std::string &message);
    void onMessage(const std::function<void(const std::string&, Connection&)> &handler);
    void onClientConnected(const std::function<void(Connection&)> &handler);
    void run();
    
private:
    int fd;
    std::vector<Connection> connections;
    std::vector<std::function<void(const std::string&, Connection&)>> handlers;
    std::vector<std::function<void(Connection&)>> connectedHandlers;
};