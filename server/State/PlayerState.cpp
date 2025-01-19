#include "PlayerState.hpp"

PlayerState::PlayerState(const std::string &name)
    : m_username(name), m_position(400, 300), m_health(100), m_score(0), m_isAlive(true)
{
}

PlayerState::PlayerState(const std::string &name, const PlayerStateUpdate &update) : m_username(name)
{
    applyUpdate(update);
}

const std::string &PlayerState::getUsername() const
{
    return m_username;
}
const sf::Vector2f &PlayerState::getPosition() const
{
    return m_position;
}
int PlayerState::getHealth() const
{
    return m_health;
}
int PlayerState::getScore() const
{
    return m_score;
}
bool PlayerState::getIsAlive() const
{
    return m_isAlive;
}

void PlayerState::applyUpdate(const PlayerStateUpdate &update)
{
    if (update.position)
        m_position = *update.position;
    if (update.health)
        m_health = *update.health;
    if (update.score)
        m_score = *update.score;
    if (update.isAlive)
        m_isAlive = *update.isAlive;
}
