#pragma once

#include <string>
#include "Notification.hpp"
#include "State/PlayerState.hpp"

class PlayerStateNotification : public Notification {
  public:
    explicit PlayerStateNotification(const std::string &id, const PlayerStateUpdate &update)
        : m_playerId(id), m_update(update)
    {
    }

    const std::string &getPlayerId() const
    {
        return m_playerId;
    }
    const PlayerStateUpdate &getUpdate() const
    {
        return m_update;
    }

  private:
    std::string m_playerId;
    PlayerStateUpdate m_update;
};
