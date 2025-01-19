#pragma once
#include <SFML/System/Vector2.hpp>
#include <optional>
#include <string>

struct BulletStateUpdate {
    std::optional<sf::Vector2f> position;
    std::optional<sf::Vector2f> velocity;

    BulletStateUpdate(
        std::optional<sf::Vector2f> position = std::nullopt, std::optional<sf::Vector2f> velocity = std::nullopt)
        : position(position), velocity(velocity)
    {
    }
};

class BulletState {
  public:
    explicit BulletState(const std::string &id, const std::string &shooterId);
    explicit BulletState(const std::string &id, const std::string &shooterId, const BulletStateUpdate &update);
    const std::string &getId() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getVelocity() const;
    const std::string &getShooterId() const;

    void applyUpdate(const BulletStateUpdate &update);

  private:
    std::string m_id;
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    std::string m_shooter;
};
