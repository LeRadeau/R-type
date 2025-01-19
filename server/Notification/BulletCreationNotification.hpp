#pragma once
#include <SFML/System/Vector2.hpp>
#include <chrono>
#include "Notification.hpp"

class BulletCreationNotification : public Notification {
  public:
    explicit BulletCreationNotification(
        const std::string &shooterId, const sf::Vector2f &position, const sf::Vector2f velocity)
        : m_bulletId(generateBulletId(shooterId)), m_shooterId(shooterId), m_position(position), m_velocity(velocity)
    {
    }

    const std::string &getBulletId() const
    {
        return m_bulletId;
    }

    const std::string &getShooterId() const
    {
        return m_shooterId;
    }

    const sf::Vector2f &getPosition() const
    {
        return m_position;
    }

    const sf::Vector2f &getVelocity() const
    {
        return m_velocity;
    }

  private:
    static std::string generateBulletId(const std::string &username)
    {
        static int counter = 0;
        std::ostringstream oss;
        auto now = std::chrono::high_resolution_clock::now();
        auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        oss << username << "_" << timestamp << "_" << counter++;
        return oss.str();
    }
    std::string m_bulletId;
    std::string m_shooterId;
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
};
