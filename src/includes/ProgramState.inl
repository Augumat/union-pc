#include <sstream>

#include "ProgramState.hpp"

namespace unn
{
template <typename T>
void ProgramState::track(T &data, const std::string &identifier)
{
    std::unique_lock l(ptrIdMtx);
    ptrToId.emplace(&data, identifier);
    idToPtr.emplace(identifier, &data);
}

template <typename T>
ProgramState &operator<<(ProgramState &state, const T &t)
{
    {
        std::shared_lock l(state.ptrIdMtx);
        if (state.ptrToId.find(&t) == state.ptrToId.end()) {
            throw std::runtime_error("Attempting to write untracked data");
        }
    }
    std::stringstream ss;
    Writer writer(ss);
    writer << t;
    ProgramState::Lock l(state.localUpdatesM);
    state.localUpdates[&t] = ss.str();
    return state;
}

template <typename T>
ProgramState &operator>>(ProgramState &state, T &t)
{
    ProgramState::Lock l(state.remoteUpdatesM);
    auto it = state.remoteUpdates.find(&t);
    if (it != state.remoteUpdates.end()) {
        std::stringstream ss(it->second);
        Reader r(ss);
        r >> t;
        state.remoteUpdates.erase(it);
    }
    return state;
}

template <typename T>
void ProgramState::untrack(T &data)
{
    std::unique_lock l(ptrIdMtx);
    auto it = ptrToId.find(&data);
    if (it != ptrToId.end()) {
        idToPtr.erase(idToPtr.find(it->second));
        ptrToId.erase(it);
        {
            Lock l(localUpdatesM);
            tryRemove(&data, localUpdates);
        }
        {
            Lock l(remoteUpdatesM);
            tryRemove(&data, remoteUpdates);
        }
    }
}

}  // namespace unn
