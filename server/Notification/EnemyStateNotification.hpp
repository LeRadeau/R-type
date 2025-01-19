#pragma once

#include "Notification.hpp"
#include "State/EnemyState.hpp"

class EnemyStateNotification : public Notification {
  public:
    explicit EnemyStateNotification(const std::string &id, const EnemyStateUpdate &update)
        : m_enemyId(id), m_update(update)
    {
    }

    const std::string &getEnemyId() const
    {
        return m_enemyId;
    }

    const EnemyStateUpdate &getUpdate() const
    {
        return m_update;
    }

  private:
    std::string m_enemyId;
    EnemyStateUpdate m_update;
};
