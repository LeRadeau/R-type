#include "PlayerStateManager.hpp"
#include "Notification/BroadcastBulletHitNotification.hpp"
#include "Notification/PlayerDeathNotification.hpp"
#include "Notification/PlayerStateNotification.hpp"

void PlayerStateManager::addPlayer(const PlayerState &player)
{
    if (m_players.find(player.getUsername()) != m_players.end())
        return;
    m_players[player.getUsername()] = player;
}
void PlayerStateManager::removePlayer(const std::string &playerId)
{
    auto player = m_players.find(playerId);

    if (player != m_players.end())
        m_players.erase(player);
}

void PlayerStateManager::onNotify(const Notification &notification)
{
    if (const auto *stateUpdate = dynamic_cast<const PlayerStateNotification *>(&notification)) {
        auto player = m_players.find(stateUpdate->getPlayerId());
        if (player != m_players.end()) {
            player->second.applyUpdate(stateUpdate->getUpdate());
        }
    }
    if (const auto *bulletHit = dynamic_cast<const BroadcastBulletHitNotification *>(&notification)) {
        auto player = m_players.find(bulletHit->getHitEntity());
        if (player != m_players.end()) {
            PlayerStateUpdate update;
            update.health = player->second.getHealth() - 10;
            player->second.applyUpdate(update);
        }
    }
}

void PlayerStateManager::update(float)
{
    NotificationBatch batch;
    for (auto i = m_players.begin(); i != m_players.end();) {
        auto &player = i->second;
        if (player.getHealth() <= 0) {
            i = m_players.erase(i);
            batch.addNotification(std::make_shared<PlayerDeathNotification>(player.getUsername()));
        } else {
            i++;
        }
    }
    notify(batch);
}

const std::unordered_map<std::string, PlayerState> &PlayerStateManager::getAllPlayers() const
{
    return m_players;
}
