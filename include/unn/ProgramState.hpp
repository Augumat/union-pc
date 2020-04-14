#pragma once
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <sstream>
#include <unordered_map>

#include "unn/Connection.hpp"
#include "unn/serialization.hpp"

namespace unn
{
class ProgramState
{
   public:
    using Lock = std::lock_guard<std::mutex>;
    ProgramState(const std::string &address, int port);

    template <typename T>
    void track(T &data, const std::string &identifier);

    template <typename T>
    friend ProgramState &operator<<(ProgramState &state, const T &t);

    template <typename T>
    friend ProgramState &operator>>(ProgramState &state, T &t);

    template <typename T>
    void untrack(T &data);
    void submit();

   private:
    void tryRemove(void *data,
                   std::unordered_map<const void *, std::string> &elem);
    void handleMessage(const std::string &msg);

    std::unordered_map<const void *, std::string> ptrToId;
    std::unordered_map<std::string, const void *> idToPtr;
    std::unordered_map<const void *, std::string> localUpdates, remoteUpdates;

    mutable std::shared_mutex ptrIdMtx;
    std::mutex localUpdatesM, remoteUpdatesM;
    Connection conn;
};

}  // namespace unn

#include "unn/ProgramState.inl"
