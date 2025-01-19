#pragma once

#include <SFML/Network/UdpSocket.hpp>
#include <SFML/System/Vector2.hpp>
#include <chrono>
#include "GameCoordinator.hpp"
#include "network/NetworkManager.hpp"

#define SERVER_PORT  54000
#define MAX_CLIENTS  4
#define MAX_ENNEMIES 10

class Server : public Observer {
  public:
    Server(unsigned short tcpPort, unsigned short udpPort, const sf::IpAddress &ip = sf::IpAddress::Any);
    ~Server();

    void init();
    void run();
    void onNotify(const Notification &notification) override;

  private:
    bool m_launchGame = false;
    std::chrono::_V2::system_clock::time_point m_previousTime;
    std::chrono::_V2::system_clock::time_point previousBulletBroadcastTime;
    std::chrono::_V2::system_clock::time_point previousClientBroadcastTime;

    Network::NetworkManager m_networkManager;
    std::atomic<bool> m_running;
    GameCoordinator m_coordinator;
    PlayerStateManager m_playerStateManager;
    PacketHandler m_packetHandler;
    EnemyStateManager m_enemyStateManager;
    BulletStateManager m_bulletStateManager;
    sf::IpAddress m_ip;
    unsigned short m_tcpPort;
    unsigned short m_udpPort;
};
