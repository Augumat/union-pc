#pragma once
#include <pthread.h>
#include <sys/socket.h>

#include <cstring>
#include <functional>
#include <string>

namespace unn
{
class Connection
{
   public:
    /* opens a connection with the specified address via the specified port */
    static Connection connect(const std::string &address, int port);

    /* sends a message through the current connection */
    void sendMessage(const std::string &message);
    /* sets a message handler for when a message is recieved */
    void onMessage(std::function<void(const std::string &)> handler);

    Connection(int fd);
    Connection(Connection &&other);
    Connection(const Connection &other) = delete;
    void start();
    int getId();

   private:
    void run();
    static void *threadRun(void *ptr);

    std::string readLen(size_t len);
    void sendData(const std::string &message);

    int fd;
    pthread_t tid;
    std::function<void(const std::string &)> msgHandler;
};

}  // namespace unn
