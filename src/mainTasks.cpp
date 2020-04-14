#include <unistd.h>

#include <iostream>
#include <string>

#include "CLI11.hpp"
#include "unn/Connection.hpp"
#include "unn/ProgramState.hpp"
#include "unn/Server.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using unn::Connection;
using unn::ProgramState;
using unn::Server;

template <typename T>
void printVec(const vector<T> &data);
bool needsMore(size_t size, size_t avSize);
string randomWord();

int runClient(const std::string &address, int port, bool observe)
{
    cout << "Connecting to " << address << ":" << port << "..." << endl;
    ProgramState state(address, port);
    vector<int> data;
    vector<string> words;
    state.track(data, "data");
    state.track(words, "words");

    while (true) {
        if (!observe) {
            if (rand() % 20 == 0) {
                if (needsMore(data.size(), 20)) {
                    data.push_back(rand() % 1000);
                } else {
                    data.erase(data.begin() + rand() % data.size());
                }
                state << data;
            }
            if (rand() % 200 == 0) {
                if (needsMore(words.size(), 6)) {
                    words.push_back(randomWord());
                } else {
                    words.erase(words.begin() + rand() % words.size());
                }
                state << words;
            }
        }
        state >> data >> words;
        state.submit();
        printVec(data);
        printVec(words);
        usleep(10000);
    }
    return 0;
}

int runServer(int port)
{
    cout << "Listening on port " << port << "..." << endl;
    unn::Server s(port);
    s.onMessage([&](const string &msg, unn::Connection &c) {
        s.broadcastMessage(msg, &c);
    });
    s.run();
    return 0;
}

bool needsMore(size_t size, size_t avSize)
{
    return rand() % ((size + 1) * (size + 1)) < avSize / 2;
}

string randomWord()
{
    const string allWords[] = {"red",    "blue",   "green",
                               "orange", "yellow", "red"};
    char buffer[32];
    const string &word =
        allWords[rand() % (sizeof(allWords) / sizeof(allWords[0]))];
    sprintf(buffer, "%s-%d", word.c_str(), rand() % 100);
    return string(buffer);
}

template <typename T>
void printVec(const vector<T> &data)
{
    std::cout << "[ ";
    for (auto &i : data) {
        std::cout << i << " ";
    }
    std::cout << "]" << std::endl;
}