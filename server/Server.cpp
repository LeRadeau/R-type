#include "Server.hpp"
#include <SFML/Network.hpp>
#include <SFML/Network/Packet.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include "./../common/Serializer.hpp"
#include "./../common/network_types.hpp"
#include <system_error>
#include <unordered_map>
#include <sstream>
#include <chrono>

#define SERVER_PORT 54000
#define MAX_CLIENTS 4
#define MAX_ENNEMIES 6


std::string generateBulletID(const std::string& username) {
    static int counter = 0;
    std::ostringstream oss;
    auto now = std::chrono::high_resolution_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    oss << username << "_" << timestamp << "_" << counter++;
    return oss.str();
}

void Server::bind(unsigned short port, const sf::IpAddress &addr)
{
    if (socket_.bind(SERVER_PORT, addr) != sf::Socket::Done) {
        throw std::system_error(errno, std::generic_category(), "Failed to bind socket");
    }
    std::cout << "Server is running on port " << port << std::endl;
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

void Server::readSocket()
{
    while (true) {
        char data[1024];
        std::size_t received;
        sf::IpAddress sender;
        unsigned short senderPort;
        if (socket_.receive(data, sizeof(data), received, sender, senderPort) != sf::Socket::Done) {
            continue;
        }
        const char *ptr = data;
        auto messageType = static_cast<MessageType>(Serializer::deserialize<uint8_t>(ptr));

        switch (messageType) {
            case MessageType::CONNECT: handleConnect(sender, senderPort, ptr); break;
            case MessageType::MOVE: handleMove(sender, senderPort, ptr); break;
            case MessageType::GOODBYE: handleGoodbye(sender, senderPort, ptr); break;
            case MessageType::SHOOT: handleShoot(sender, senderPort, ptr); break;
            default: break;
        }
    }
}

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

void Server::updateBullets(float deltaTime)
{
    for (auto it = bullets_.begin(); it != bullets_.end();) {
        it->position.x += it->velocity.x * deltaTime;
        it->position.y += it->velocity.y * deltaTime;

        if (it->position.x < 0 || it->position.x > 1920 || it->position.y < 0 || it->position.y > 1080) {
            it = bullets_.erase(it);
        } else {
            ++it;
        }
    }
}

void Server::updateEnnemies(float deltaTime)
{
    for (auto &ennemy : ennemies_) {
        if (ennemy.isAlive) {
            ennemy.position.x += ennemy.velocity.x * deltaTime;
            ennemy.position.y += ennemy.velocity.y * deltaTime;
        } else {
            ennemy.respawnCooldown -= deltaTime;
            if (ennemy.respawnCooldown <= 0) {
                ennemy.isAlive = true;
                ennemy.health = 100;
            }
        }
    }
}

void Server::loadEnnemies()
{
    for (int i = 0; i < MAX_ENNEMIES; ++i) {
        Ennemy newEnnemy;
        newEnnemy.id = "ennemy_" + std::to_string(i);
        newEnnemy.position = {float(1000), float(100 * i)};
        newEnnemy.velocity = {0, 0};
        newEnnemy.health = 100;
        newEnnemy.shootingCooldown = 1.0f;
        newEnnemy.respawnCooldown = 5.0f;
        newEnnemy.isAlive = true;
        ennemies_.push_back(newEnnemy);
    }
}

void Server::CheckEnnemyCollision()
{
    for (auto &ennemy : ennemies_) {
        for (auto &bullet : bullets_) {
            if (ennemy.isAlive && bullet.shooter != "ennemy" && bullet.position.x > ennemy.position.x - 20 && bullet.position.x < ennemy.position.x + 20 && bullet.position.y > ennemy.position.y - 20 && bullet.position.y < ennemy.position.y + 20) {
                ennemy.health -= 10;
                if (ennemy.health <= 0) {
                    ennemy.isAlive = false;
                    ennemy.respawnCooldown = 5.0f;
                }
                bullet.position = {2000, 2000};
            }
        }
    }
}

void Server::run()
{
    std::thread networkThread(&Server::readSocket, this);
    auto previousTime = std::chrono::high_resolution_clock::now();
    auto previousBulletBroadcastTime = previousTime;
    auto previousClientBroadcastTime = previousTime;

    loadEnnemies();
    while (true) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - previousTime;
        previousTime = currentTime;
        float deltaTimeSeconds = deltaTime.count();

        updateBullets(deltaTimeSeconds);
        updateEnnemies(deltaTimeSeconds);
        CheckEnnemyCollision();

        if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousClientBroadcastTime).count() >= 50) {
            broadcastClients();
            broadcastEnnemies();
            previousClientBroadcastTime = currentTime;
        }

        if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousBulletBroadcastTime).count() >= 50) {
            broadcastBullet();
            previousBulletBroadcastTime = currentTime;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

int main()
{
    Server server;

    try {
        server.bind(SERVER_PORT);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    server.run();
}
