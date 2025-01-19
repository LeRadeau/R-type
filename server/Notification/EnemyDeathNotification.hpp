#pragma once

#include "Notification.hpp"

class EnemyDeathNotification : public Notification {
  public:
    explicit EnemyDeathNotification(const std::string &id) : m_enemyId(id)
    {
    }

    const std::string &getEnemyId() const
    {
        return m_enemyId;
    }

  private:
    std::string m_enemyId;
};
