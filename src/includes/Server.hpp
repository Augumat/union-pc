#pragma once
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "Connection.hpp"

namespace unn
{
class Server
{
   public:
    Server(int port);
    void broadcastMessage(const std::string &message,
                          Connection *ignore = NULL);
    void onMessage(
        const std::function<void(const std::string &, Connection &)> &handler);
    void onClientConnected(const std::function<void(Connection &)> &handler);
    void run();

   private:
    int fd;
    std::mutex connectionsM;
    std::vector<std::unique_ptr<Connection>> connections;
    std::vector<std::function<void(const std::string &, Connection &)>>
        handlers;
    std::vector<std::function<void(Connection &)>> connectedHandlers;
};
}  // namespace unn
