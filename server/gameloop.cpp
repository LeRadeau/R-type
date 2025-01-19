#include <chrono>
#include <thread>
#include "Server.hpp"

void Server::run()
{
    auto previousSpawnTime = m_previousTime;

    m_networkManager.listen(m_ip, m_udpPort, m_tcpPort);

    std::thread(&PacketHandler::handleIncomingPackets, this, running_).detach();

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
