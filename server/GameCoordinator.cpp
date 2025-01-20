#include "GameCoordinator.hpp"
#include <cmath>
#include "Notification/BroadcastBulletHitNotification.hpp"
#include "Notification/EnemyStateNotification.hpp"
#include "Notification/Notification.hpp"
#include "Notification/PlayerStateNotification.hpp"
#include "State/EnemyState.hpp"

GameCoordinator::GameCoordinator(PacketHandler &packetHandler, PlayerStateManager &playerStateManager,
    EnemyStateManager &enemyStateManager, BulletStateManager &bulletStateManager)
    : m_playerStateManager(playerStateManager), m_enemyStateManager(enemyStateManager),
      m_bulletStateManager(bulletStateManager), m_packetHandler(packetHandler)
{
}

void GameCoordinator::init()
{
    m_enemyStateManager.addObserver(&m_bulletStateManager);
    m_enemyStateManager.addObserver(&m_packetHandler);
    m_packetHandler.addObserver(&m_bulletStateManager);
    m_packetHandler.addObserver(&m_playerStateManager);
    addObserver(&m_bulletStateManager);
    addObserver(&m_playerStateManager);
    addObserver(&m_packetHandler);
    addObserver(&m_enemyStateManager);
}

void GameCoordinator::update(float deltaTime)
{
    m_playerStateManager.update(deltaTime);
    m_enemyStateManager.update(deltaTime);
    m_bulletStateManager.update(deltaTime);

    handleBulletCollisions();
}

static bool checkCircleCollision(float x1, float y1, float r1, float x2, float y2, float r2)
{
    float dx = x1 - x2;
    float dy = y1 - y2;
    float distanceSquared = dx * dx + dy * dy;
    float radiiSum = r1 + r2;
    float radiiDiff = std::fabs(r1 - r2);

    return distanceSquared <= (radiiSum * radiiSum) && distanceSquared >= (radiiDiff * radiiDiff);
}

void GameCoordinator::handleBulletCollisions()
{
    auto bullets = m_bulletStateManager.getAllBullets();
    auto players = m_playerStateManager.getAllPlayers();
    auto enemies = m_enemyStateManager.getAllEnemies();
    NotificationBatch batch;

    for (auto it = bullets.begin(); it != bullets.end(); it++) {
        const auto &bullet = it->second;
        float bulletX = bullet.getPosition().x;
        float bulletY = bullet.getPosition().y;
        float bulletRadius = 5;
        if (bullet.getShooterId() == "enemy") {
            for (auto &playerIt : players) {
                const auto &player = playerIt.second;
                float playerX = player.getPosition().x;
                float playerY = player.getPosition().y;
                float clientRadius = 32;

                if (!player.getIsAlive())
                    continue;
                if (checkCircleCollision(bulletX, bulletY, bulletRadius, playerX, playerY, clientRadius)) {
                    batch.addNotification(
                        std::make_shared<BroadcastBulletHitNotification>(bullet.getId(), player.getUsername()));
                    PlayerStateUpdate update;
                    update.health = player.getHealth() - 10;
                    batch.addNotification(std::make_shared<PlayerStateNotification>(player.getUsername(), update));
                    break;
                }
            }
        } else {
            for (auto &enemyIt : enemies) {
                auto enemy = enemyIt.second;
                float enemyX = enemy.getPosition().x;
                float enemyY = enemy.getPosition().y;
                float enemyRadius = 32;

                if (!enemy.isAlive())
                    continue;
                if (checkCircleCollision(bulletX, bulletY, bulletRadius, enemyX, enemyY, enemyRadius)) {
                    batch.addNotification(
                        std::make_shared<BroadcastBulletHitNotification>(bullet.getId(), enemy.getId()));
                    EnemyStateUpdate update;
                    update.health = enemy.getHealth() - 10;
                    batch.addNotification(std::make_shared<EnemyStateNotification>(enemy.getId(), update));
                    if (update.health <= 0) {
                        PlayerStateUpdate update;
                        for (auto i : players) {
                            if (i.first == bullet.getShooterId()) {
                                update.score = i.second.getScore() + 10;
                                batch.addNotification(std::make_shared<PlayerStateNotification>(i.first, update));
                            }
                        }
                    }

                    break;
                }
            }
        }
    }
    notify(batch);
}
