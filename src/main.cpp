#include <unistd.h>

#include <iostream>
#include <string>

#include <CLI11.hpp>
#include "mainTasks.hpp"

int main(int argc, char **argv)
{
    CLI::App app{"App description"};

    std::string address = "";
    int port = 46466;
    bool isServer = false, observe = false;

    app.add_option("address", address, "Address to connect to");
    app.add_flag("-s,--server", isServer, "Run as server");
    app.add_flag("-o,--observe", observe, "Only observe");
    app.add_option("-p,--port", port, "Port");

    CLI11_PARSE(app, argc, argv);

    if (address.empty() != isServer) {
        std::cerr << argv[0]
                  << ": error: ip must be specified when acting as a client"
                  << std::endl;
        return 1;
    }

    if (isServer) {
        return runServer(port);
    } else {
        return runClient(address, port, observe);
    }
}
