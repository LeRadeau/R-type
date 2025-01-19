#pragma once
#include <string>
#include "Observer.hpp"
#include "State/PlayerState.hpp"
#include "StateManager.hpp"
#include <unordered_map>

class PlayerStateManager : public Observer, Subject, StateManager {
  public:
    void addPlayer(const PlayerState &player);
    void removePlayer(const std::string &playerId);

    void onNotify(const Notification &notification) override;
    void update(float deltaTime) override;

    const std::unordered_map<std::string, PlayerState> &getAllPlayers() const;

  private:
    mutable std::mutex m_mutex;
    std::unordered_map<std::string, PlayerState> m_players;
};
