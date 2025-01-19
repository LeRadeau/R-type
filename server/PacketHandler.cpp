#include "PacketHandler.hpp"
#include "Notification/BroadcastBulletHitNotification.hpp"
#include "Notification/BulletCreationNotification.hpp"
#include "Notification/EnemyDeathNotification.hpp"
#include "Notification/GameStartNotification.hpp"
#include "Notification/PlayerCreationNotification.hpp"
#include "Notification/PlayerDeathNotification.hpp"
#include "Notification/PlayerStateNotification.hpp"
#include "State/PlayerState.hpp"
#include "StateManager/PlayerStateManager.hpp"
#include "network/packets/BulletHitPacket.hpp"
#include "network/packets/BulletsUpdatePacket.hpp"
#include "network/packets/EnemiesUpdatePacket.hpp"
#include "network/packets/EnemyDeathPacket.hpp"
#include "network/packets/ErrorPacket.hpp"
#include "network/packets/GameOverPacket.hpp"
#include "network/packets/GameStartPacket.hpp"
#include "network/packets/PlayerConnectPacket.hpp"
#include "network/packets/PlayerDeathPacket.hpp"
#include "network/packets/PlayerDisconnectPacket.hpp"
#include "network/packets/PlayerMovePacket.hpp"
#include "network/packets/PlayerShootPacket.hpp"
#include "network/packets/PlayersUpdatePacket.hpp"

void PacketHandler::onNotify(const Notification &notification)
{
    if (const auto *playerDeath = dynamic_cast<const PlayerDeathNotification *>(&notification)) {
        for (auto i : m_clients)
            m_networkManager.sendPacket(
                std::make_shared<Network::PlayerDeathPacket>(playerDeath->getPlayerId()), i.second.socket);
    }
    if (const auto *enemyDeath = dynamic_cast<const EnemyDeathNotification *>(&notification)) {
        for (auto i : m_clients)
            m_networkManager.sendPacket(
                std::make_shared<Network::EnemyDeathPacket>(enemyDeath->getEnemyId()), i.second.socket);
    }
    if (const auto *bulletHit = dynamic_cast<const BroadcastBulletHitNotification *>(&notification)) {
        for (auto i : m_clients)
            m_networkManager.sendPacket(
                std::make_shared<Network::BulletHitPacket>(bulletHit->getBulletId()), i.second.socket);
    }
}

void PacketHandler::broadcastClients(const PlayerStateManager &playerStateManager)
{
    auto players = playerStateManager.getAllPlayers();

    size_t totalClients = players.size();
    size_t totalPacketSize = sizeof(Network::Packet::PacketType) + sizeof(totalClients)
        + totalClients * sizeof(Network::PlayersUpdatePacket::ClientData);

    std::vector<Network::PlayersUpdatePacket::ClientData> data;

    for (auto &i : players) {
        auto &client = i.second;
        data.push_back({client.getUsername(), client.getPosition().x, client.getPosition().y, client.getHealth(),
            client.getScore()});
    }

    auto packet = std::make_shared<Network::PlayersUpdatePacket>(data);
    for (auto &i : m_clients) {
        auto &client = i.second;
        m_networkManager.sendPacket(packet, client.ip, client.port);
    }
}

void PacketHandler::broadcastBullets(const BulletStateManager &bulletStateManager)
{
    auto bullets_ = bulletStateManager.getAllBullets();
    size_t totalBullets = bullets_.size();
    size_t totalPacketSize = sizeof(Network::Packet::PacketType) + sizeof(totalBullets)
        + totalBullets * sizeof(Network::BulletsUpdatePacket::BulletData);
    std::vector<Network::BulletsUpdatePacket::BulletData> data;
    size_t packetSize = sizeof(size_t) + sizeof(Network::Packet::PacketType) + sizeof(size_t);

    auto it = bullets_.begin();
    while (it != bullets_.end()) {
        for (; it != bullets_.end(); it++) {
            const auto &bullet = it->second;
            packetSize += sizeof(Network::BulletsUpdatePacket::BulletData) + bullet.getId().size();
            if (packetSize >= MAX_PACKET_SIZE) {
                packetSize = sizeof(size_t) + sizeof(Network::Packet::PacketType) + sizeof(size_t);
                break;
            }
            data.push_back({bullet.getId(), bullet.getPosition().x, bullet.getPosition().y, bullet.getVelocity().x,
                bullet.getVelocity().y});
        }
        if (data.empty())
            break;
        auto packet = std::make_shared<Network::BulletsUpdatePacket>(data);
        for (auto &i : m_clients) {
            auto &client = i.second;
            m_networkManager.sendPacket(packet, client.ip, client.port);
        }
        data.clear();
    }
}

void PacketHandler::broadcastEnnemies(const EnemyStateManager &enemyStateManager)
{
    auto enemies_ = enemyStateManager.getAllEnemies();

    size_t totalEnemies = enemies_.size();
    size_t packetSize = sizeof(size_t) + sizeof(Network::Packet::PacketType) + sizeof(size_t);
    std::vector<Network::EnemiesUpdatePacket::EnemyData> data;

    auto it = enemies_.begin();
    while (it != enemies_.end()) {
        for (; it != enemies_.end(); it++) {
            const auto &enemy = it->second;
            packetSize += sizeof(Network::EnemiesUpdatePacket::EnemyData) + enemy.getId().size();
            if (packetSize >= MAX_PACKET_SIZE) {
                packetSize = sizeof(size_t) + sizeof(Network::Packet::PacketType) + sizeof(size_t);
                break;
            }
            data.push_back({enemy.getId(), enemy.getPosition().x, enemy.getPosition().y, enemy.getHealth()});
        }
        if (data.empty())
            break;
        auto packet = std::make_shared<Network::EnemiesUpdatePacket>(data);
        for (auto &i : m_clients) {
            const auto &client = i.second;
            m_networkManager.sendPacket(packet, client.ip, client.port);
        }
    }
}

void PacketHandler::broadcastGameOver()
{
    auto packet = std::make_shared<Network::GameOverPacket>();
    for (const auto &i : m_clients) {
        const auto &client = i.second;
        m_networkManager.sendPacket(packet, client.socket);
    }
}

void PacketHandler::handleReady(const Network::NetworkManager::NetworkPacketInfo &packet)
{
    bool shouldContinue = false;
    for (auto &i : m_clients) {
        if (i.second.socket == packet.socket) {
            shouldContinue = true;
        }
    }
    if (!shouldContinue)
        return;
    if (m_clients.size() >= 1) {
        auto packet = std::make_shared<Network::GameStartPacket>();
        for (auto it = m_clients.begin(); it != m_clients.end(); it++) {
            m_networkManager.sendPacket(packet, it->second.socket);
        }
        notify(GameStartNotification());
        /* Ready = true; */
        /* previousTime = std::chrono::high_resolution_clock::now(); */
        /* previousBulletBroadcastTime = previousTime; */
        /* previousClientBroadcastTime = previousTime; */
    }
}

void PacketHandler::handleConnect(const Network::NetworkManager::NetworkPacketInfo &packet)
{
    auto connectPacket = std::dynamic_pointer_cast<Network::PlayerConnectPacket>(packet.packet);

    if (!connectPacket || !packet.socket.has_value())
        return;
    std::string newUsername = connectPacket->getUsername();
    if (m_clients.find(newUsername) != m_clients.end()) {
        auto errorPacket = std::make_shared<Network::ErrorPacket>("Username already exists");
        m_networkManager.sendPacket(errorPacket, packet.socket.value());
    }
    if (m_clients.size() >= MAX_CLIENTS) {
        auto errorPacket = std::make_shared<Network::ErrorPacket>("PacketHandler is full");
        m_networkManager.sendPacket(errorPacket, packet.socket.value());
    }
    m_clients[newUsername] = ClientNetworking(packet.socket.value(), connectPacket->getPort());
    notify(PlayerCreationNotification(newUsername));
}

void PacketHandler::handleMove(const Network::NetworkManager::NetworkPacketInfo &packet)
{
    auto playerMovePacket = std::dynamic_pointer_cast<Network::PlayerMovePacket>(packet.packet);
    if (!playerMovePacket)
        return;
    PlayerStateUpdate update;
    sf::Vector2f position(playerMovePacket->getX(), playerMovePacket->getX());
    update.position = position;
    notify(PlayerStateNotification(playerMovePacket->getUsername(), update));
}

void PacketHandler::handleDisconnect(const Network::NetworkManager::NetworkPacketInfo &packet)
{
    auto disconnectPacket = std::dynamic_pointer_cast<Network::PlayerDisconnectPacket>(packet.packet);

    if (!disconnectPacket)
        return;
    PlayerStateUpdate update;
    update.isAlive = false;
    notify(PlayerStateNotification(disconnectPacket->getUsername(), update));
}

void PacketHandler::handleShoot(const Network::NetworkManager::NetworkPacketInfo &packet)
{
    auto playerShootPacket = std::dynamic_pointer_cast<Network::PlayerShootPacket>(packet.packet);

    if (!playerShootPacket)
        return;
    sf::Vector2f position(playerShootPacket->getX(), playerShootPacket->getY());
    notify(BulletCreationNotification(playerShootPacket->getUsername(), position));
}
