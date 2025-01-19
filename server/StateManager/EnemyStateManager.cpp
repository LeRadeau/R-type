#include "EnemyStateManager.hpp"
#include <cmath>
#include "Notification/BulletCreationNotification.hpp"
#include "Notification/EnemyDeathNotification.hpp"
#include "Notification/EnemyStateNotification.hpp"
#include "Notification/Notification.hpp"
#include "State/EnemyState.hpp"

EnemyStateManager::EnemyStateManager() : m_level(1)
{
}

void EnemyStateManager::addEnemy(const EnemyState &enemy)
{
    if (m_enemies.find(enemy.getId()) != m_enemies.end())
        return;
    m_enemies.emplace(enemy.getId(), enemy);
}

void EnemyStateManager::removeEnemy(const std::string &enemyId)
{
    auto it = m_enemies.find(enemyId);
    if (it != m_enemies.end())
        m_enemies.erase(it);
}

void EnemyStateManager::update(float deltaTime)
{
    NotificationBatch batch;

    spawnEnemies(m_level / 1000);
    if (m_level / 1000 < 10)
        m_level += deltaTime;
    auto it = m_enemies.begin();
    while (it != m_enemies.end()) {
        auto &enemy = it->second;

        if (enemy.isAlive()) {
            auto position = enemy.getPosition();
            auto velocity = enemy.getVelocity();
            auto startingY = enemy.getStartingY();
            auto amplitude = enemy.getAmplitude();
            auto cosinus = enemy.getCosinus();
            auto frequency = enemy.getFrequency();
            auto shootingCooldown = enemy.getShootingCooldown();

            position.x -= (velocity.x) * deltaTime;
            position.y = startingY + amplitude * std::sin(cosinus * frequency);
            cosinus += deltaTime;
            shootingCooldown -= deltaTime;

            EnemyStateUpdate update;
            update.position = position;
            update.cosinus = cosinus;
            update.shootingCooldown = shootingCooldown;

            if (shootingCooldown <= 0) {
                batch.addNotification(
                    std::make_shared<BulletCreationNotification>("enemy", enemy.getPosition(), sf::Vector2f(-500, 0)));
                update.shootingCooldown = 2.0f;
            }
            enemy.applyUpdate(update);
            it++;
        } else {
            batch.addNotification(std::make_shared<EnemyDeathNotification>(it->second.getId()));
            it = m_enemies.erase(it);
        }
    }
    notify(batch);
}

void EnemyStateManager::onNotify(const Notification &notification)
{
    if (const auto *enemyState = dynamic_cast<const EnemyStateNotification *>(&notification)) {
        auto enemy = m_enemies.find(enemyState->getEnemyId());
        if (enemy != m_enemies.end())
            enemy->second.applyUpdate(enemyState->getUpdate());
    }
}

const std::unordered_map<std::string, EnemyState> &EnemyStateManager::getAllEnemies() const
{
    return m_enemies;
}

void EnemyStateManager::spawnEnemies(int level)
{
    static int enemyCounter = 0;
    float lastSpawnX = 1920.0f;
    const float minSpacingX = 150.0f;

    for (int i = 0; i < level; ++i) {
        std::string id = "enemy_" + std::to_string(enemyCounter++);
        EnemyStateUpdate update;
        float screenHeight = 1080.0f;
        sf::Vector2f position;
        position.x = lastSpawnX + minSpacingX;
        position.y = static_cast<float>(rand() % static_cast<int>(screenHeight - 20)) + 10.0f;
        lastSpawnX = position.x;
        sf::Vector2f velocity = {50, 15};
        update.velocity = velocity;
        update.health = 100;
        update.shootingCooldown = static_cast<float>((rand() % 3) + 1);
        update.isAlive = true;
        update.amplitude = 70.0f;
        update.frequency = 1.0f;
        update.cosinus = 1;
        update.startingY = position.y;
        EnemyState newEnemy(id, update);
        addEnemy(newEnemy);
    }
}
