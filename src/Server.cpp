#include "Server.hpp"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace unn
{
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
        struct sockaddr_in clientAddr = {0, 0};
        socklen_t len = 0;
        int clientSock =
            accept(fd, (struct sockaddr *)&clientAddr, (socklen_t *)&len);
        std::lock_guard<std::mutex> l(connectionsM);
        connections.emplace_back(std::make_unique<Connection>(clientSock));
        Connection *conn = &*connections.back();
        for (auto &i : connectedHandlers) {
            i(*conn);
        }
        conn->onMessage([=](const std::string &message) {
            for (auto &handler : handlers) {
                handler(message, *conn);
            }
        });
        conn->start();
    }
}

void Server::broadcastMessage(const std::string &message, Connection *ignore)
{
    std::unique_lock<std::mutex> l(connectionsM);
    for (auto &connection : connections) {
        if (&*connection != ignore) {
            connection->sendMessage(message);
        }
    }
}

void Server::onMessage(
    const std::function<void(const std::string &, Connection &)> &handler)
{
    handlers.emplace_back(handler);
}

void Server::onClientConnected(const std::function<void(Connection &)> &handler)
{
    connectedHandlers.emplace_back(handler);
}

}  // namespace unn