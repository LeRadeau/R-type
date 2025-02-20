#include <SFML/Network.hpp>
#include <chrono>
#include <iostream>
#include <sstream>
#include "Serializer.hpp"
#include "Server.hpp"
#include "network_types.hpp"

std::string Server::generateBulletID(const std::string &username)
{
    static int counter = 0;
    std::ostringstream oss;
    auto now = std::chrono::high_resolution_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    oss << username << "_" << timestamp << "_" << counter++;
    return oss.str();
}

void Server::handleReady(const sf::IpAddress &sender, unsigned short senderPort, char const *&ptr)
{
    bool shouldContinue = false;
    for (auto &i : clients_) {
        if (i.second.ip == sender && i.second.port == senderPort) {
            shouldContinue = true;
        }
    }
    if (!shouldContinue)
        return;
    // Check if there is at less 1 player
    if (clients_.size() >= 1) {
        for (auto it = clients_.begin(); it != clients_.end(); it++) {
            std::string buffer;
            Serializer::serialize(buffer, static_cast<uint8_t>(MessageType::START_GAME));
            socket_.send(buffer.data(), buffer.size(), it->second.ip, it->second.port);
            std::cout << "Server: Send START_GAME to client" << it->second.ip << std::endl;
        }
        // The server start the game !
        Ready = true;
        previousTime = std::chrono::high_resolution_clock::now();
        previousBulletBroadcastTime = previousTime;
        previousClientBroadcastTime = previousTime;
    }
}

void Server::handleConnect(const sf::IpAddress &sender, unsigned short senderPort, char const *&ptr)
{
    std::string newUsername = Serializer::deserializeString(ptr);
    std::cout << "CONNECTION by " << sender.toString() << ":" << senderPort << std::endl;
    if (clients_.find(newUsername) != clients_.end()) {
        std::cout << "Client already exists: " << newUsername << std::endl;

        std::string errorBuffer;
        Serializer::serialize(errorBuffer, static_cast<uint8_t>(MessageType::ERROR));
        Serializer::serialize(errorBuffer, "Username already exists");
        socket_.send(errorBuffer.data(), errorBuffer.size(), sender, senderPort);
        std::cout << "Server: Send MessageType::ERROR already exist to client" << sender << std::endl;
    }
    if (clients_.size() >= MAX_CLIENTS) {
        std::cout << "Server is full" << std::endl;

        std::string errorBuffer;
        Serializer::serialize(errorBuffer, static_cast<uint8_t>(MessageType::ERROR));
        Serializer::serialize(errorBuffer, "Server is full");
        socket_.send(errorBuffer.data(), errorBuffer.size(), sender, senderPort);
        std::cout << "Server: Send MessageType::ERROR FULL to client" << sender << std::endl;
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

    Bullet newBullet;
    newBullet.id = generateBulletID(shootingUsername);
    newBullet.position = {x, y};
    newBullet.velocity = {500, 0};
    newBullet.shooter = shootingUsername;

    bullets_.push_back(newBullet);
}
