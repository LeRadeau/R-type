#include "Server.hpp"
#include "../common/Serializer.hpp"
#include "../common/network_types.hpp"

static const size_t MAX_PACKET_SIZE = 1000;
    
void Server::broadcastClients()
{
    std::string buffer;
    size_t totalClients = clients_.size();
    size_t clientsSerialized = 0;

    while (clientsSerialized < totalClients) {
        buffer.clear();
        Serializer::serialize(buffer, static_cast<uint8_t>(MessageType::UPDATE_CLIENTS));
        size_t packetClientsCount = 0;
        size_t bufferStartSize = buffer.size();

        auto it = clients_.begin();
        std::advance(it, clientsSerialized);

        for (size_t i = clientsSerialized; i < totalClients; ++i, ++it) {
            std::string tempBuffer;
            const auto &client = it->second;
            Serializer::serialize(tempBuffer, client.username);
            Serializer::serialize(tempBuffer, client.position.x);
            Serializer::serialize(tempBuffer, client.position.y);

            if (buffer.size() + tempBuffer.size() > MAX_PACKET_SIZE) {
                break;
            }

            buffer += tempBuffer;
            ++packetClientsCount;
        }

        Serializer::serialize(buffer, static_cast<uint32_t>(packetClientsCount));
        buffer.insert(buffer.begin() + sizeof(uint8_t), reinterpret_cast<char*>(&packetClientsCount), reinterpret_cast<char*>(&packetClientsCount) + sizeof(uint32_t));

        for (const auto &client : clients_) {
            socket_.send(buffer.data(), buffer.size(), client.second.ip, client.second.port);
        }

        clientsSerialized += packetClientsCount;
    }
}

void Server::broadcastBullet()
{
    std::string buffer;
    size_t totalBullets = bullets_.size();
    size_t bulletsSerialized = 0;

    while (bulletsSerialized < totalBullets) {
        buffer.clear();
        Serializer::serialize(buffer, static_cast<uint8_t>(MessageType::UPDATE_BULLETS));
        size_t packetBulletsCount = 0;
        size_t bufferStartSize = buffer.size();

        auto it = bullets_.begin();
        std::advance(it, bulletsSerialized);

        for (size_t i = bulletsSerialized; i < totalBullets; ++i, ++it) {
            std::string tempBuffer;
            const auto &bullet = *it;
            Serializer::serialize(tempBuffer, bullet.id);
            Serializer::serialize(tempBuffer, bullet.position.x);
            Serializer::serialize(tempBuffer, bullet.position.y);
            Serializer::serialize(tempBuffer, bullet.velocity.x);
            Serializer::serialize(tempBuffer, bullet.velocity.y);

            if (buffer.size() + tempBuffer.size() > MAX_PACKET_SIZE) {
                break;
            }

            buffer += tempBuffer;
            ++packetBulletsCount;
        }

        Serializer::serialize(buffer, static_cast<uint32_t>(packetBulletsCount));
        buffer.insert(buffer.begin() + sizeof(uint8_t), reinterpret_cast<char*>(&packetBulletsCount), reinterpret_cast<char*>(&packetBulletsCount) + sizeof(uint32_t));

        for (const auto &client : clients_) {
            socket_.send(buffer.data(), buffer.size(), client.second.ip, client.second.port);
        }

        bulletsSerialized += packetBulletsCount;
    }
}

void Server::broadcastEnnemies()
{
    std::string buffer;
    size_t totalEnemies = ennemies_.size();
    size_t enemiesSerialized = 0;

    while (enemiesSerialized < totalEnemies) {
        buffer.clear();
        Serializer::serialize(buffer, static_cast<uint8_t>(MessageType::UPDATE_ENEMIES));
        size_t packetEnemiesCount = 0;
        size_t bufferStartSize = buffer.size();

        auto it = ennemies_.begin();
        std::advance(it, enemiesSerialized);

        for (size_t i = enemiesSerialized; i < totalEnemies; ++i, ++it) {
            std::string tempBuffer;
            const auto &ennemy = *it;
            Serializer::serialize(tempBuffer, ennemy.id);
            Serializer::serialize(tempBuffer, ennemy.position.x);
            Serializer::serialize(tempBuffer, ennemy.position.y);
            Serializer::serialize(tempBuffer, ennemy.health);

            if (buffer.size() + tempBuffer.size() > MAX_PACKET_SIZE) {
                break;
            }

            buffer += tempBuffer;
            ++packetEnemiesCount;
        }

        Serializer::serialize(buffer, static_cast<uint32_t>(packetEnemiesCount));
        buffer.insert(buffer.begin() + sizeof(uint8_t), reinterpret_cast<char*>(&packetEnemiesCount), reinterpret_cast<char*>(&packetEnemiesCount) + sizeof(uint32_t));

        for (const auto &client : clients_) {
            // std::cout << "Sending " << packetEnemiesCount << " enemies to " << client.second.username << std::endl;
            socket_.send(buffer.data(), buffer.size(), client.second.ip, client.second.port);
        }

        enemiesSerialized += packetEnemiesCount;
    }
}