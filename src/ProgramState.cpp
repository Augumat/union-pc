#include "unn/ProgramState.hpp"

#include <iostream>

namespace unn
{
ProgramState::ProgramState(const std::string &address, int port)
    : conn(Connection::connect(address, port))
{
    conn.onMessage([&](const std::string &message) { handleMessage(message); });
    conn.start();
}

void ProgramState::submit()
{
    Lock l(localUpdatesM);
    for (auto &item : localUpdates) {
        std::stringstream ss;
        Writer w(ss);
        {
            SharedLock l(ptrIdMtx);
            auto identifierIt = ptrToId.find(item.first);
            if (identifierIt == ptrToId.end()) {
                throw std::runtime_error("Untracked item updated");
            }
            w << identifierIt->second << item.second;
        }
        conn.sendMessage(ss.str());
    }
    localUpdates.clear();
}

void ProgramState::tryRemove(
    void *data, std::unordered_map<const void *, std::string> &elem)
{
    auto it = elem.find(&data);
    if (it != elem.end()) {
        elem.erase(it);
    }
}

void ProgramState::handleMessage(const std::string &msg)
{
    std::stringstream ss(msg);
    Reader r(ss);
    std::string identifier, data;
    r >> identifier >> data;
    SharedLock l(ptrIdMtx);
    auto it = idToPtr.find(identifier);
    if (it != idToPtr.end()) {
        Lock l(remoteUpdatesM);
        remoteUpdates[it->second] = data;
    }
}
}  // namespace unn
