#include "BulletStateManager.hpp"
#include "Notification/BroadcastBulletHitNotification.hpp"
#include "Notification/BulletCreationNotification.hpp"
#include "Notification/Notification.hpp"

void BulletStateManager::addBullet(const BulletState &bullet)
{
    if (m_bullets.find(bullet.getId()) != m_bullets.end())
        return;
    m_bullets.emplace(bullet.getId(), bullet);
}

void BulletStateManager::removeBullet(const std::string &bulletId)
{
    auto bullet = m_bullets.find(bulletId);

    if (bullet != m_bullets.end())
        m_bullets.erase(bullet);
}

void BulletStateManager::update(float deltaTime)
{
    for (auto it = m_bullets.begin(); it != m_bullets.end();) {
        auto &bullet = it->second;

        sf::Vector2f newPosition = bullet.getPosition();
        newPosition.x += bullet.getVelocity().x * deltaTime;
        newPosition.y += bullet.getVelocity().y * deltaTime;

        if (newPosition.x < 0 || newPosition.x > 1920 || newPosition.y < 0 || newPosition.y > 1080) {
            it = m_bullets.erase(it);
        } else {
            BulletStateUpdate update(newPosition);
            it->second.applyUpdate(update);
            ++it;
        }
    }
}

void BulletStateManager::onNotify(const Notification &notification)
{
    if (const auto *bulletCreation = dynamic_cast<const BulletCreationNotification *>(&notification)) {
        BulletState newBullet(bulletCreation->getBulletId(), bulletCreation->getShooterId());
        BulletStateUpdate update;
        update.position = bulletCreation->getPosition();
        update.velocity = {-500.0f, 0.0f};
        newBullet.applyUpdate(update);
        addBullet(newBullet);
    }
    if (const auto *bulletHit = dynamic_cast<const BroadcastBulletHitNotification *>(&notification)) {
        removeBullet(bulletHit->getBulletId());
    }
}

const std::unordered_map<std::string, BulletState> &BulletStateManager::getAllBullets() const
{
    return m_bullets;
}
