#pragma once
#include "Notification.hpp"
#include "State/BulletState.hpp"

class BulletStateNotification : public Notification {
  public:
    explicit BulletStateNotification(const std::string &id, const BulletStateUpdate &update)
        : m_bulletId(id), m_update(update)
    {
    }

    const std::string &getBulletId() const
    {
        return m_bulletId;
    }

    const BulletStateUpdate &getUpdate() const
    {
        return m_update;
    }

  private:
    std::string m_bulletId;
    BulletStateUpdate m_update;
};
