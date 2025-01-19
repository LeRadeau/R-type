#pragma once
#include <SFML/System/Vector2.hpp>
#include <optional>
#include <string>

struct EnemyStateUpdate {
    std::optional<sf::Vector2f> position;
    std::optional<sf::Vector2f> velocity;
    std::optional<int> health;
    std::optional<float> shootingCooldown;
    std::optional<float> startingY;
    std::optional<bool> isAlive;
    std::optional<float> amplitude;
    std::optional<float> frequency;
    std::optional<float> cosinus;
};

class EnemyState {
  public:
    EnemyState(const std::string &id);
    EnemyState(const std::string &id, const EnemyStateUpdate &update);

    void applyUpdate(const EnemyStateUpdate &update);

    const std::string &getId() const;
    const sf::Vector2f &getPosition() const;
    const sf::Vector2f &getVelocity() const;
    int getHealth() const;
    float getShootingCooldown() const;
    float getStartingY() const;
    bool isAlive() const;
    float getAmplitude() const;
    float getFrequency() const;
    float getCosinus() const;

  private:
    std::string m_id;
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    int m_health;
    float m_shootingCooldown;
    float m_startingY;
    bool m_isAlive;
    float m_amplitude;
    float m_frequency;
    float m_cosinus;
};
