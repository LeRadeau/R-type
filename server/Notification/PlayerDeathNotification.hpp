#include "Notification.hpp"

class PlayerDeathNotification : public Notification {
  public:
    explicit PlayerDeathNotification(const std::string &id) : m_playerId(id)
    {
    }

    const std::string &getPlayerId() const
    {
        return m_playerId;
    }

  private:
    std::string m_playerId;
};
