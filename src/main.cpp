#include <unistd.h>
#include "Server.hpp"
#include <iostream>
#include <string>
#include "CLI11.hpp"
#include "Connection.hpp"

using namespace std;

void clientMsgHandler(const string &msg) {
    //stub
    cout << "client received: " << msg << endl;
}

void serverMsgHandler(const string &msg, Connection& c) {
    //stub
    cout << "server received " << msg << " from client " << c.getId() << endl;
}

void serverOnClientConnected(Connection& c) {
    //stub
    cout << "client connected: " << c.getId() << endl;
}

int main(int argc, char** argv) {
    CLI::App app{"App description"};

    string ip = "";
    int port = 46466;
    bool isServer = false;
    app.add_option("ip", ip, "IP Address");
    app.add_option("isServer", isServer, "Whether or not this is the server");
    app.add_option("-p,--port", port, "Port");

    CLI11_PARSE(app, argc, argv);

    if (ip == "") {
        cout << "IP not found" << endl;
        return 1;
    }

    if (isServer) {
        Server s(port);
        s.onMessage(serverMsgHandler);
        s.onClientConnected(serverOnClientConnected);
        s.run();

    } else {
        Connection c = Connection::connect(ip, port);
        c.onMessage(clientMsgHandler);
        c.start();
        
        string msg1 = "First message!";
        string msg2 = "Second message...";
        c.sendMessage(msg1);
        c.sendMessage(msg2);
        sleep(10);
    }
    
    return 0;
}
