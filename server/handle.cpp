#include <SFML/Network.hpp>
#include <iostream>
#include <chrono>
#include <sstream>
#include "Server.hpp"
#include "../common/Serializer.hpp"
#include "../common/network_types.hpp"

std::string Server::generateBulletID(const std::string& username) {
    static int counter = 0;
    std::ostringstream oss;
    auto now = std::chrono::high_resolution_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    oss << username << "_" << timestamp << "_" << counter++;
    return oss.str();
}

void Server::handleConnect(const sf::IpAddress &sender, unsigned short senderPort, char const *&ptr)
{
    std::string newUsername = Serializer::deserializeString(ptr);

    if (clients_.find(newUsername) != clients_.end()) {
        std::cout << "Client already exists: " << newUsername << std::endl;

        std::string errorBuffer;
        Serializer::serialize(errorBuffer, static_cast<uint8_t>(MessageType::ERROR));
        Serializer::serialize(errorBuffer, "Username already exists");
        socket_.send(errorBuffer.data(), errorBuffer.size(), sender, senderPort);
    }
    if (clients_.size() >= MAX_CLIENTS) {
        std::cout << "Server is full" << std::endl;

        std::string errorBuffer;
        Serializer::serialize(errorBuffer, static_cast<uint8_t>(MessageType::ERROR));
        Serializer::serialize(errorBuffer, "Server is full");
        socket_.send(errorBuffer.data(), errorBuffer.size(), sender, senderPort);
    }

    clients_[newUsername] = Client(sender, senderPort, newUsername);
    std::cout << "New client connected: " << newUsername << std::endl;
}

void Server::handleMove(const sf::IpAddress &sender, unsigned short senderPort, char const *&ptr)
{
    std::string movingUsername = Serializer::deserializeString(ptr);
    float x = Serializer::deserialize<float>(ptr);
    float y = Serializer::deserialize<float>(ptr);

    if (clients_.find(movingUsername) != clients_.end()) {
        clients_[movingUsername].position = {x, y};
    }
}

void Server::handleGoodbye(const sf::IpAddress &sender, unsigned short senderPort, char const *&ptr)
{
    std::string leavingUsername = Serializer::deserializeString(ptr);

    if (clients_.find(leavingUsername) != clients_.end()) {
        clients_.erase(leavingUsername);
        std::cout << "Client disconnected: " << leavingUsername << std::endl;
    }
}

void Server::handleShoot(const sf::IpAddress &sender, unsigned short senderPort, char const *&ptr)
{
    std::string shootingUsername = Serializer::deserializeString(ptr);
    float x = Serializer::deserialize<float>(ptr);
    float y = Serializer::deserialize<float>(ptr);

    std::cout << shootingUsername << " shot at " << x << ", " << y << std::endl;

    Bullet newBullet;
    newBullet.id = generateBulletID(shootingUsername);
    newBullet.position = {x, y};
    newBullet.velocity = {500, 0};
    newBullet.shooter = shootingUsername;

    bullets_.push_back(newBullet);
}