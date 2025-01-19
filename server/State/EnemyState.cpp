#include "EnemyState.hpp"

EnemyState::EnemyState(const std::string &id) : m_id(id)
{
}

EnemyState::EnemyState(const std::string &id, const EnemyStateUpdate &update) : m_id(id)
{
    applyUpdate(update);
}

void EnemyState::applyUpdate(const EnemyStateUpdate &update)
{
    if (update.position)
        m_position = *update.position;
    if (update.velocity)
        m_velocity = *update.velocity;
    if (update.health)
        m_health = *update.health;
    if (update.shootingCooldown)
        m_shootingCooldown = *update.shootingCooldown;
    if (update.startingY)
        m_startingY = *update.startingY;
    if (update.isAlive)
        m_isAlive = *update.isAlive;
    if (update.amplitude)
        m_amplitude = *update.amplitude;
    if (update.frequency)
        m_frequency = *update.frequency;
    if (update.cosinus)
        m_cosinus = *update.cosinus;
}

const std::string &EnemyState::getId() const
{
    return m_id;
}
const sf::Vector2f &EnemyState::getPosition() const
{
    return m_position;
}
const sf::Vector2f &EnemyState::getVelocity() const
{
    return m_velocity;
}
int EnemyState::getHealth() const
{
    return m_health;
}
float EnemyState::getShootingCooldown() const
{
    return m_shootingCooldown;
}
float EnemyState::getStartingY() const
{
    return m_startingY;
}
bool EnemyState::isAlive() const
{
    return m_isAlive;
}
float EnemyState::getAmplitude() const
{
    return m_amplitude;
}
float EnemyState::getFrequency() const
{
    return m_frequency;
}
float EnemyState::getCosinus() const
{
    return m_cosinus;
}
