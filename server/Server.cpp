#include "Server.hpp"
#include "Notification/GameStartNotification.hpp"

Server::Server(unsigned short tcpPort, unsigned short udpPort, const sf::IpAddress &ip)
    : m_networkManager(Network::NetworkManager::Mode::SERVER), m_tcpPort(tcpPort), m_udpPort(udpPort), m_ip(ip),
      m_packetHandler(m_networkManager),
      m_coordinator(m_packetHandler, m_playerStateManager, m_enemyStateManager, m_bulletStateManager)
{
}

Server::~Server()
{
    running_ = false;
    m_networkManager.stop();
}

void Server::onNotify(const Notification &notification)
{
    if (const auto *gameStart = dynamic_cast<const GameStartNotification *>(&notification)) {
        m_launchGame = true;
    }
}
