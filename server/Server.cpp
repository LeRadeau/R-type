#include "Server.hpp"
#include <chrono>
#include <thread>
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

void Server::run()
{
    auto previousSpawnTime = m_previousTime;

    m_networkManager.listen(m_ip, m_udpPort, m_tcpPort);

    std::thread(&PacketHandler::handleIncomingPackets, &m_packetHandler, std::ref(running_)).detach();

    while (running_) {
        if (m_launchGame == false) {
            m_packetHandler.broadcastWait();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Attends 1 seconde avant le prochain check
            continue;
        }
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - m_previousTime;
        m_previousTime = currentTime;
        float deltaTimeSeconds = deltaTime.count();

        m_coordinator.update(deltaTimeSeconds);

        if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousClientBroadcastTime).count()
            >= 16) {
            m_packetHandler.broadcastClients(m_playerStateManager);
            m_packetHandler.broadcastEnnemies(m_enemyStateManager);
            previousClientBroadcastTime = currentTime;
        }

        if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousBulletBroadcastTime).count()
            >= 16) {
            m_packetHandler.broadcastBullets(m_bulletStateManager);
            previousBulletBroadcastTime = currentTime;
        }

        int isAlive = 0;
        for (auto &player : m_playerStateManager.getAllPlayers()) {
            isAlive += player.second.getIsAlive();
        }
        if (!isAlive) {
            m_packetHandler.broadcastGameOver();
            running_ = false;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}
