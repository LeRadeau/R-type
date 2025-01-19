#pragma once
#include "Notification.hpp"

class BroadcastBulletHitNotification : public Notification {
  public:
    explicit BroadcastBulletHitNotification(const std::string &bulletId, const std::string &hitEntityId)
        : m_bulletId(bulletId), m_hitEntityId(hitEntityId)
    {
    }

    const std::string &getBulletId() const
    {
        return m_bulletId;
    }

    const std::string &getHitEntity() const
    {
        return m_hitEntityId;
    }

  private:
    std::string m_bulletId;
    std::string m_hitEntityId;
};
