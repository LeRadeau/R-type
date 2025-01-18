#pragma once

#include <SFML/Network/UdpSocket.hpp>
#include <SFML/System/Vector2.hpp>
#include <chrono>
#include <list>
#include "Bullet.hpp"
#include "Client.hpp"
#include "Enemy.hpp"
#include "network/NetworkManager.hpp"
#include <unordered_map>

#define SERVER_PORT  54000
#define MAX_CLIENTS  4
#define MAX_ENNEMIES 10

class Server {
  public:
    void run();
    Server(unsigned short tcpPort, unsigned short udpPort, const sf::IpAddress &ip = sf::IpAddress::Any);
    ~Server();

  private:
    void handleReady(const Network::NetworkManager::NetworkPacketInfo &packet);
    void handleConnect(const Network::NetworkManager::NetworkPacketInfo &packet);
    void handleMove(const Network::NetworkManager::NetworkPacketInfo &packet);
    void handleDisconnect(const Network::NetworkManager::NetworkPacketInfo &packet);
    void handleShoot(const Network::NetworkManager::NetworkPacketInfo &packet);
    void handleIncomingPackets();
    void broadcastClients();
    void broadcastBullet();
    void broadcastEnnemies();
    void broadcastBulletHit(const std::string &bulletId);
    void broadcastEnemyDeath(const Enemy &enemy);
    void broadcastPlayerDeath(const std::string &username);
    void broadcastGameOver();
    void updateBullets(float deltaTime);
    void updateEnemies(float deltaTime);
    void CheckBulletCollisions();
    void enemyShoot(Enemy &enemy);
    void spawnEnnemies(int count);
    std::string generateBulletID(const std::string &username);
    std::unordered_map<std::string, Client> clients_;
    std::list<Bullet> bullets_;
    std::list<Enemy> enemies_;
    bool Ready = false; // Booléen qui permet de savoir si la game est lancé
    std::chrono::_V2::system_clock::time_point previousTime;
    std::chrono::_V2::system_clock::time_point previousBulletBroadcastTime;
    std::chrono::_V2::system_clock::time_point previousClientBroadcastTime;

    Network::NetworkManager m_networkManager;
    sf::IpAddress m_ip;
    unsigned short m_tcpPort;
    unsigned short m_udpPort;

    std::atomic<bool> running_;
};
