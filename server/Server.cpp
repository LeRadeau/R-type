#include "Server.hpp"
#include <chrono>
#include <thread>
#include "Notification/GameStartNotification.hpp"

Server::Server(unsigned short tcpPort, unsigned short udpPort, const sf::IpAddress &ip)
    : m_networkManager(Network::NetworkManager::Mode::SERVER),
      m_coordinator(m_packetHandler, m_playerStateManager, m_enemyStateManager, m_bulletStateManager),
      m_packetHandler(m_networkManager), m_ip(ip), m_tcpPort(tcpPort), m_udpPort(udpPort)
{
}

Server::~Server()
{
    m_running = false;
    m_networkManager.stop();
}

void Server::onNotify(const Notification &notification)
{
    if (dynamic_cast<const GameStartNotification *>(&notification)) {
        m_launchGame = true;
    }
}

void Server::init()
{
    m_running = true;
    m_coordinator.init();
    m_networkManager.listen(m_ip, m_udpPort, m_tcpPort);
    m_networkManager.start();
    m_packetHandler.addObserver(this);
    std::thread(&PacketHandler::handleIncomingPackets, &m_packetHandler, std::ref(m_running)).detach();
    /* std::thread(&PacketHandler::handleNetworkErrors, &m_packetHandler, std::ref(m_running)).detach(); */
}

void Server::run()
{
    auto previousTime = std::chrono::high_resolution_clock::now();
    auto previousBulletBroadcastTime = previousTime;
    auto previousClientBroadcastTime = previousTime;

    constexpr std::chrono::milliseconds updateIntervals(16); // 16ms for ~60 updates per second

    while (m_running) {
        if (m_launchGame == false) {
            m_packetHandler.broadcastWait();
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Check every seconds
            continue;
        }

        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        float deltaTimeSeconds = deltaTime.count();

        m_coordinator.update(deltaTimeSeconds);

        if (currentTime - previousClientBroadcastTime >= updateIntervals) {
            m_packetHandler.broadcastClients(m_playerStateManager);
            m_packetHandler.broadcastEnnemies(m_enemyStateManager);
            previousClientBroadcastTime = currentTime;
        }

        if (currentTime - previousBulletBroadcastTime >= updateIntervals) {
            m_packetHandler.broadcastBullets(m_bulletStateManager);
            previousBulletBroadcastTime = currentTime;
        }

        int isAlive = 0;
        for (auto &player : m_playerStateManager.getAllPlayers()) {
            isAlive += player.second.getIsAlive();
        }
        if (!isAlive) {
            m_packetHandler.broadcastGameOver();
            m_running = false;
            break;
        }

        auto elapsedTime = std::chrono::high_resolution_clock::now() - currentTime;
        auto sleepTime = updateIntervals - std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime);
        if (sleepTime > std::chrono::milliseconds::zero())
            std::this_thread::sleep_for(sleepTime);
    }
}
