#include <chrono>
#include <thread>
#include "Server.hpp"
#include <iostream>

void Server::run()
{
    std::thread networkThread(&Server::readSocket, this);
    auto previousTime = std::chrono::high_resolution_clock::now();
    auto previousBulletBroadcastTime = previousTime;
    auto previousClientBroadcastTime = previousTime;

    auto previousSpawnTime = previousTime;
    int level = 1;

    // loadEnnemies();
    while (true) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - previousTime;
        previousTime = currentTime;
        float deltaTimeSeconds = deltaTime.count();

        updateBullets(deltaTimeSeconds);
        updateEnnemies(deltaTimeSeconds);
        CheckEnnemyCollision();

        if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousSpawnTime).count() >= 10000) {
            level++;
            spawnEnnemies(level);
            previousSpawnTime = currentTime;
        }
        std::cout << "spawning: " << level << " ennemies!"<< std::endl;
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