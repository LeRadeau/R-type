#include <chrono>
#include <iostream>
#include <thread>
#include "Server.hpp"
#include "network/packets/WaitingUpdatePacket.hpp"

void Server::run()
{
    auto previousSpawnTime = previousTime;
    int level = 1;

    m_networkManager.listen(m_ip, m_udpPort, m_tcpPort);

    std::thread(&Server::handleIncomingPackets, this).detach();

    while (true) {
        // Vérifie si le jeu est ready
        if (Ready == false) {
            auto packet = std::make_shared<Network::WaitingUpdatePacket>(clients_.size());
            for (auto it = clients_.begin(); it != clients_.end(); it++) {
                m_networkManager.sendPacket(packet, it->second.ip, it->second.port);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Attends 1 seconde avant le prochain check
            continue;
        }
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - previousTime;
        previousTime = currentTime;
        float deltaTimeSeconds = deltaTime.count();

        updateBullets(deltaTimeSeconds);
        updateEnemies(deltaTimeSeconds);
        CheckBulletCollisions();

        if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousSpawnTime).count() >= 10000) {
            if (level < 10)
                level++;
            spawnEnnemies(level);
            previousSpawnTime = currentTime;
            std::cout << "spawning: " << level << " ennemies!" << std::endl;
        }

        if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousClientBroadcastTime).count()
            >= 16) {
            broadcastClients();
            broadcastEnnemies();
            previousClientBroadcastTime = currentTime;
        }

        if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousBulletBroadcastTime).count()
            >= 16) {
            broadcastBullet();
            previousBulletBroadcastTime = currentTime;
        }

        int isAlive = 0;
        for (auto &client : clients_) {
            isAlive += client.second.isAlive;
        }
        if (!isAlive) {
            broadcastGameOver();
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}
