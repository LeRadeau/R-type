#pragma once

#include "PacketHandler.hpp"
#include "StateManager/BulletStateManager.hpp"
#include "StateManager/EnemyStateManager.hpp"
#include "StateManager/PlayerStateManager.hpp"

class GameCoordinator : public Subject {
  public:
    GameCoordinator(PacketHandler &, PlayerStateManager &, EnemyStateManager &, BulletStateManager &);
    void update(float deltaTime);

  private:
    void handleBulletCollisions();

    PlayerStateManager &m_playerStateManager;
    EnemyStateManager &m_enemyStateManager;
    BulletStateManager &m_bulletStateManager;
    PacketHandler &m_packetHandler;
};
