#include "Server.hpp"

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>

Server::Server(int port) : fd(socket(AF_INET, SOCK_STREAM, 0))
{
    int opt = 1;
    struct sockaddr_in address;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    bind(fd, (struct sockaddr *)&address, sizeof(address));
    listen(fd, 3);
}

void Server::run()
{
    while (true) {
        struct sockaddr_in clientAddr;
        socklen_t len;
        std::cout << "Waiting for connection..." << std::endl;
        int clientSock = accept(fd, (struct sockaddr *)&clientAddr, (socklen_t *)&len);
        std::cout << "Got connection: " << clientSock << std::endl;
        connections.emplace_back(clientSock);
        Connection *conn = &connections.back();
        conn->onMessage([&](const std::string &message) {
            for (auto &handler : handlers) {
                handler(message, *conn);
            }
        });
        conn->start();
    }
}

void Server::broadcastMessage(const std::string &message)
{
    for (auto &connection : connections) {
        connection.sendMessage(message);
    }
}

void Server::onMessage(const std::function<void(const std::string &, Connection &)> &handler)
{
    handlers.emplace_back(handler);
}

void Server::onClientConnected(const std::function<void(Connection &)> &handler)
{
    connectedHandlers.emplace_back(handler);
}
