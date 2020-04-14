#include "Connection.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <sstream>

#define LEN_BYTES 8

namespace unn
{
Connection Connection::connect(const std::string &address, int port)
{
    int sock = 0;
    struct sockaddr_in addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, address.c_str(), &addr.sin_addr) <= 0) {
        throw std::runtime_error("Invalid address/ Address not supported");
    }

    if (::connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        throw std::runtime_error("Connection Failed");
    }
    return Connection(sock);
}

void Connection::start()
{
    pthread_create(&tid, NULL, Connection::threadRun, this);
}

void *Connection::threadRun(void *ptr)
{
    ((Connection *)ptr)->run();
    return NULL;
}

std::string sizeToBytes(size_t paramInt)
{
    std::string bytes(LEN_BYTES, '\0');
    for (int i = 0; i < LEN_BYTES; i++) {
        bytes[i] = (paramInt >> (i * LEN_BYTES));
    }
    return bytes;
}

size_t bytesToSize(std::string bytes)
{
    assert(bytes.size() == LEN_BYTES);
    size_t len = 0;
    for (int i = 0; i < LEN_BYTES; i++) {
        len |= bytes[i] << (i * LEN_BYTES);
    }
    return len;
}

void Connection::sendMessage(const std::string &message)
{
    std::string lenMsg = sizeToBytes(message.size());
    sendData(lenMsg);
    sendData(message);
}

void Connection::onMessage(std::function<void(const std::string &)> newHandler)
{
    msgHandler = newHandler;
}

Connection::Connection(int fd) : fd(fd) {}

int Connection::getId() { return fd; }

void Connection::run()
{
    while (true) {
        std::string lenStr = readLen(LEN_BYTES);
        size_t len = bytesToSize(lenStr);
        std::string message = readLen(len);
        msgHandler(message);
    }
}

std::string Connection::readLen(size_t len)
{
    size_t numRead = 0;
    std::stringstream ss;
    while (numRead < len) {
        char buffer[1024];
        ssize_t readSize =
            read(fd, buffer, std::min((size_t)1024, len - numRead));
        numRead += readSize;
        if (readSize == 0) {  // TODO: Handle closing
            throw std::runtime_error("Connection closed");
        }
        if (readSize < 0) {
            throw std::runtime_error("Connection error");
        }
        ss << std::string(buffer, buffer + readSize);
        buffer[1023] = '\0';
    }
    return ss.str();
}

void Connection::sendData(const std::string &message)
{
    const char *data = message.c_str();
    size_t len = message.size();
    size_t lenWritten = 0;
    while (lenWritten < len) {
        size_t written = send(fd, data + lenWritten, len - lenWritten, 0);
        if (written == 0) {
            throw std::runtime_error("Client closed connection");
        }
        lenWritten += written;
    }
}

}  // namespace unn
