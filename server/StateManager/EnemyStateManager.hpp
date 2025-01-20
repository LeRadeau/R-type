#pragma once

#include <string>
#include "Observer.hpp"
#include "State/EnemyState.hpp"
#include "StateManager.hpp"
#include <unordered_map>

class EnemyStateManager : public StateManager, public Subject, public Observer {
  public:
    EnemyStateManager();
    void addEnemy(const EnemyState &);
    void removeEnemy(const std::string &);
    void update(float deltaTime) override;

    void onNotify(const Notification &notification) override;

    const std::unordered_map<std::string, EnemyState> &getAllEnemies() const;

  private:
    void spawnEnemies(int level);
    std::unordered_map<std::string, EnemyState> m_enemies;
    int m_level;
    float m_spawnCooldown;
};
