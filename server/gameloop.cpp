#include <chrono>
#include <thread>
#include "Server.hpp"
#include "../common/Serializer.hpp"
#include "./../common/network_types.hpp"
#include <iostream>

void Server::run()
{
    std::thread networkThread(&Server::readSocket, this);
    auto previousTime = std::chrono::high_resolution_clock::now();
    auto previousBulletBroadcastTime = previousTime;
    auto previousClientBroadcastTime = previousTime;

    loadEnnemies();
    while (true) {
        // Vérifie si le jeu est ready
        if (Ready == false) {
            for (auto it = clients_.begin(); it != clients_.end(); it++) {
                std::string buffer;
                Serializer::serialize(buffer, static_cast<uint8_t>(MessageType::WAIT));
                Serializer::serialize(buffer, static_cast<std::size_t>(clients_.size()));
                socket_.send(buffer.data(), buffer.size(), it->second.ip, it->second.port);
                std::cout << "hein\n";
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Attends 1 seconde avant le prochain check
            continue;
        }
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