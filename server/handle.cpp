#include <SFML/Network.hpp>
#include <chrono>
#include <iostream>
#include <memory>
#include "Server.hpp"
#include "network/packets/ErrorPacket.hpp"
#include "network/packets/GameStartPacket.hpp"
#include "network/packets/PlayerConnectPacket.hpp"
#include "network/packets/PlayerDisconnectPacket.hpp"
#include "network/packets/PlayerMovePacket.hpp"
#include "network/packets/PlayerShootPacket.hpp"

std::string Server::generateBulletID(const std::string &username)
{
    static int counter = 0;
    std::ostringstream oss;
    auto now = std::chrono::high_resolution_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    oss << username << "_" << timestamp << "_" << counter++;
    return oss.str();
}

void Server::handleReady(const Network::NetworkManager::NetworkPacketInfo &packet)
{
    bool shouldContinue = false;
    for (auto &i : clients_) {
        if (i.second.socket == packet.socket) {
            shouldContinue = true;
        }
    }
    if (!shouldContinue)
        return;
    if (clients_.size() >= 1) {
        auto packet = std::make_shared<Network::GameStartPacket>();
        for (auto it = clients_.begin(); it != clients_.end(); it++) {
            m_networkManager.sendPacket(packet, it->second.socket);
        }
        Ready = true;
        previousTime = std::chrono::high_resolution_clock::now();
        previousBulletBroadcastTime = previousTime;
        previousClientBroadcastTime = previousTime;
    }
}

void Server::handleConnect(const Network::NetworkManager::NetworkPacketInfo &packet)
{
    auto connectPacket = std::dynamic_pointer_cast<Network::PlayerConnectPacket>(packet.packet);

    if (!connectPacket || !packet.socket.has_value())
        return;
    std::string newUsername = connectPacket->getUsername();
    if (clients_.find(newUsername) != clients_.end()) {
        std::cout << "Client already exists: " << newUsername << std::endl;
        auto errorPacket = std::make_shared<Network::ErrorPacket>("Username already exists");
        m_networkManager.sendPacket(errorPacket, packet.socket.value());
    }
    if (clients_.size() >= MAX_CLIENTS) {
        auto errorPacket = std::make_shared<Network::ErrorPacket>("Server is full");
        m_networkManager.sendPacket(errorPacket, packet.socket.value());
    }
    clients_[newUsername] = Client(packet.socket.value(), connectPacket->getPort(), newUsername);
}

void Server::handleMove(const Network::NetworkManager::NetworkPacketInfo &packet)
{
    auto playerMovePacket = std::dynamic_pointer_cast<Network::PlayerMovePacket>(packet.packet);
    if (!playerMovePacket)
        return;

    if (clients_.find(playerMovePacket->getUsername()) != clients_.end()) {
        clients_[playerMovePacket->getUsername()].position = {playerMovePacket->getX(), playerMovePacket->getY()};
    }
}

void Server::handleDisconnect(const Network::NetworkManager::NetworkPacketInfo &packet)
{
    auto disconnectPacket = std::dynamic_pointer_cast<Network::PlayerDisconnectPacket>(packet.packet);

    if (!disconnectPacket)
        return;
    if (clients_.find(disconnectPacket->getUsername()) != clients_.end()) {
        clients_.erase(disconnectPacket->getUsername());
        std::cout << "Client disconnected: " << disconnectPacket->getUsername() << std::endl;
    }
}

void Server::handleShoot(const Network::NetworkManager::NetworkPacketInfo &packet)
{
    auto playerShootPacket = std::dynamic_pointer_cast<Network::PlayerShootPacket>(packet.packet);

    if (!playerShootPacket)
        return;

    Bullet newBullet;
    newBullet.id = generateBulletID(playerShootPacket->getUsername());
    newBullet.position = {playerShootPacket->getX(), playerShootPacket->getY()};
    newBullet.velocity = {500, 0};
    newBullet.shooter = playerShootPacket->getUsername();

    bullets_.push_back(newBullet);
}
