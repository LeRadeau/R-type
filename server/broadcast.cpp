#include "Server.hpp"
#include "network/packets/BulletHitPacket.hpp"
#include "network/packets/BulletsUpdatePacket.hpp"
#include "network/packets/EnemiesUpdatePacket.hpp"
#include "network/packets/EnemyDeathPacket.hpp"
#include "network/packets/GameOverPacket.hpp"
#include "network/packets/PlayerDeathPacket.hpp"
#include "network/packets/PlayersUpdatePacket.hpp"

static const size_t MAX_PACKET_SIZE = 1000;

void Server::broadcastClients()
{
    size_t totalClients = clients_.size();
    size_t totalPacketSize = sizeof(Network::Packet::PacketType) + sizeof(totalClients)
        + totalClients * sizeof(Network::PlayersUpdatePacket::ClientData);

    std::vector<Network::PlayersUpdatePacket::ClientData> data;
    for (auto &i : clients_) {
        auto &client = i.second;
        data.push_back({client.username, client.position.x, client.position.y, client.health, client.score});
    }
    auto packet = std::make_shared<Network::PlayersUpdatePacket>(data);
    for (auto &i : clients_) {
        auto &client = i.second;
        m_networkManager.sendPacket(packet, client.ip, client.port);
    }
}

void Server::broadcastBullet()
{
    size_t totalBullets = bullets_.size();
    size_t totalPacketSize = sizeof(Network::Packet::PacketType) + sizeof(totalBullets)
        + totalBullets * sizeof(Network::BulletsUpdatePacket::BulletData);
    std::vector<Network::BulletsUpdatePacket::BulletData> data;
    size_t packetSize = sizeof(size_t) + sizeof(Network::Packet::PacketType) + sizeof(size_t);

    auto bullet = bullets_.begin();
    while (bullet != bullets_.end()) {
        for (; bullet != bullets_.end(); bullet++) {
            packetSize += sizeof(Network::BulletsUpdatePacket::BulletData) + bullet->id.size();
            if (packetSize >= MAX_PACKET_SIZE) {
                packetSize = sizeof(size_t) + sizeof(Network::Packet::PacketType) + sizeof(size_t);
                break;
            }
            data.push_back(
                {bullet->id, bullet->position.x, bullet->position.y, bullet->velocity.x, bullet->velocity.y});
        }
        if (data.empty())
            break;
        auto packet = std::make_shared<Network::BulletsUpdatePacket>(data);
        for (auto &i : clients_) {
            auto &client = i.second;
            m_networkManager.sendPacket(packet, client.ip, client.port);
        }
        data.clear();
    }
}

void Server::broadcastEnnemies()
{
    size_t totalEnemies = enemies_.size();
    size_t packetSize = sizeof(size_t) + sizeof(Network::Packet::PacketType) + sizeof(size_t);
    std::vector<Network::EnemiesUpdatePacket::EnemyData> data;

    auto enemy = enemies_.begin();
    while (enemy != enemies_.end()) {
        for (; enemy != enemies_.end(); enemy++) {
            packetSize += sizeof(Network::EnemiesUpdatePacket::EnemyData) + enemy->id.size();
            if (packetSize >= MAX_PACKET_SIZE) {
                packetSize = sizeof(size_t) + sizeof(Network::Packet::PacketType) + sizeof(size_t);
                break;
            }
            data.push_back({enemy->id, enemy->position.x, enemy->position.y, enemy->health});
        }
        if (data.empty())
            break;
        auto packet = std::make_shared<Network::EnemiesUpdatePacket>(data);
        for (auto &i : clients_) {
            auto &client = i.second;
            m_networkManager.sendPacket(packet, client.ip, client.port);
        }
    }
}

void Server::broadcastBulletHit(const std::string &bulletId)
{
    auto packet = std::make_shared<Network::BulletHitPacket>(bulletId);
    for (const auto &i : clients_) {
        const auto &client = i.second;
        m_networkManager.sendPacket(packet, client.ip, client.port);
    }
}

void Server::broadcastEnemyDeath(const Enemy &enemy)
{
    auto packet = std::make_shared<Network::EnemyDeathPacket>(enemy.id);
    for (const auto &i : clients_) {
        const auto &client = i.second;
        m_networkManager.sendPacket(packet, client.ip, client.port);
    }
}

void Server::broadcastPlayerDeath(const std::string &username)
{
    auto packet = std::make_shared<Network::PlayerDeathPacket>(username);
    for (const auto &i : clients_) {
        const auto &client = i.second;
        m_networkManager.sendPacket(packet, client.ip, client.port);
    }
}

void Server::broadcastGameOver()
{
    auto packet = std::make_shared<Network::GameOverPacket>();
    for (const auto &i : clients_) {
        const auto &client = i.second;
        m_networkManager.sendPacket(packet, client.ip, client.port);
    }
}
