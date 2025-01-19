#include "BulletState.hpp"

BulletState::BulletState(const std::string &id, const std::string &shooterId) : m_id(id), m_shooter(shooterId)
{
}

BulletState::BulletState(const std::string &id, const std::string &shooterId, const BulletStateUpdate &update)
    : m_id(id), m_shooter(shooterId)
{
    applyUpdate(update);
}

const std::string &BulletState::getId() const
{
    return m_id;
}

sf::Vector2f BulletState::getPosition() const
{
    return m_position;
}

sf::Vector2f BulletState::getVelocity() const
{
    return m_velocity;
}

const std::string &BulletState::getShooterId() const
{
    return m_shooter;
}

void BulletState::applyUpdate(const BulletStateUpdate &update)
{
    if (update.velocity)
        m_velocity = *update.velocity;
    if (update.position)
        m_position = *update.position;
}
