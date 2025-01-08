#include "Server.hpp"
#include "../common/Serializer.hpp"
#include "../common/network_types.hpp"

void Server::broadcastClients()
{
    std::string buffer;
    Serializer::serialize(buffer, static_cast<uint8_t>(MessageType::UPDATE_CLIENTS));
    Serializer::serialize(buffer, static_cast<uint32_t>(clients_.size()));

    for (const auto &client : clients_) {
        Serializer::serialize(buffer, client.second.username);
        Serializer::serialize(buffer, client.second.position.x);
        Serializer::serialize(buffer, client.second.position.y);
    }

    for (const auto &client : clients_) {
        
        socket_.send(buffer.data(), buffer.size(), client.second.ip, client.second.port);
    }
}

void Server::broadcastBullet()
{
    std::string buffer;
    Serializer::serialize(buffer, static_cast<uint8_t>(MessageType::UPDATE_BULLETS));
    Serializer::serialize(buffer, static_cast<uint32_t>(bullets_.size()));

    for (const auto &bullet : bullets_) {
        Serializer::serialize(buffer, bullet.id);
        Serializer::serialize(buffer, bullet.position.x);
        Serializer::serialize(buffer, bullet.position.y);
        Serializer::serialize(buffer, bullet.velocity.x);
        Serializer::serialize(buffer, bullet.velocity.y);
    }

    for (const auto &client : clients_) {
        socket_.send(buffer.data(), buffer.size(), client.second.ip, client.second.port);
    }
}

void Server::broadcastEnnemies()
{
    std::string buffer;
    Serializer::serialize(buffer, static_cast<uint8_t>(MessageType::UPDATE_ENEMIES));
    Serializer::serialize(buffer, static_cast<uint32_t>(ennemies_.size()));

    for (const auto &ennemy : ennemies_) {
        Serializer::serialize(buffer, ennemy.id);
        Serializer::serialize(buffer, ennemy.position.x);
        Serializer::serialize(buffer, ennemy.position.y);
        Serializer::serialize(buffer, ennemy.health);
    }

    for (const auto &client : clients_) {
        socket_.send(buffer.data(), buffer.size(), client.second.ip, client.second.port);
    }
}