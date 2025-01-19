#pragma once

#include "Client.hpp"
#include "Observer.hpp"
#include "StateManager/BulletStateManager.hpp"
#include "StateManager/EnemyStateManager.hpp"
#include "StateManager/PlayerStateManager.hpp"
#include "network/NetworkManager.hpp"
#include <unordered_map>

#define MAX_CLIENTS     4
#define MAX_PACKET_SIZE 1000

class PacketHandler : public Subject, public Observer {
  public:
    explicit PacketHandler(Network::NetworkManager &networkManager);
    void handleIncomingPackets();

    void onNotify(const Notification &notification) override;
    void broadcastClients(const PlayerStateManager &playerStateManager);
    void broadcastBullets(const BulletStateManager &bulletStateManager);
    void broadcastEnnemies(const EnemyStateManager &enemyStateManager);
    void broadcastGameOver();

  private:
    void handleReady(const Network::NetworkManager::NetworkPacketInfo &packet);
    void handleConnect(const Network::NetworkManager::NetworkPacketInfo &packet);
    void handleMove(const Network::NetworkManager::NetworkPacketInfo &packet);
    void handleDisconnect(const Network::NetworkManager::NetworkPacketInfo &packet);
    void handleShoot(const Network::NetworkManager::NetworkPacketInfo &packet);

  private:
    Network::NetworkManager &m_networkManager;
    std::unordered_map<std::string, ClientNetworking> m_clients;
};
