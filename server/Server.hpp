#pragma once

#include <SFML/Network/UdpSocket.hpp>
#include <SFML/System/Vector2.hpp>
#include <chrono>
#include <list>
#include <thread>
#include <unordered_map>

#define SERVER_PORT  54000
#define MAX_CLIENTS  4
#define MAX_ENNEMIES 10

struct Client {
    sf::IpAddress ip;
    unsigned short port;
    std::string username;
    sf::Vector2f position;
    int health;
    int score;
    bool isAlive;

    Client() : ip(), port(0), username(""), position(0, 0), health(0), score(0), isAlive(false)
    {
    }
    Client(sf::IpAddress ipAddr, unsigned short p, const std::string &name)
        : ip(ipAddr), port(p), username(name), position(0, 0), health(100), score(0), isAlive(true)
    {
    }
};

struct Bullet {
    std::string id;
    sf::Vector2f position;
    sf::Vector2f velocity;
    std::string shooter;
};

struct Ennemy {
    std::string id;
    sf::Vector2f position;
    sf::Vector2f velocity;
    int health;
    float shootingCooldown;
    float respawnCooldown;
    float startingY;
    bool isAlive;
    float amplitude;
    float frequency;
    float cosinus;
};

class Server {
  public:
    void bind(unsigned short port, const sf::IpAddress &addr = sf::IpAddress::Any);
    void run();
    ~Server();

  private:
    void handleReady(const sf::IpAddress &sender, unsigned short senderPort, char const *&ptr);
    void handleConnect(const sf::IpAddress &sender, unsigned short senderPort, char const *&ptr);
    void handleMove(const sf::IpAddress &sender, unsigned short senderPort, char const *&ptr);
    void handleGoodbye(const sf::IpAddress &sender, unsigned short senderPort, char const *&ptr);
    void handleShoot(const sf::IpAddress &sender, unsigned short senderPort, char const *&ptr);
    void readSocket();
    void broadcastClients();
    void broadcastBullet();
    void broadcastEnnemies();
    void broadcastBulletHit(const std::string &bulletId);
    void broadcastEnemyDeath(const Ennemy &enemy);
    void broadcastPlayerDeath(const std::string &username);
    void broadcastGameOver();
    void updateBullets(float deltaTime);
    void updateEnnemies(float deltaTime);
    void loadEnnemies();
    void CheckBulletCollisions();
    void enemyShoot(Ennemy &ennemy);
    void spawnEnnemies(int count);
    std::string generateBulletID(const std::string &username);
    sf::UdpSocket socket_;
    std::unordered_map<std::string, Client> clients_;
    std::list<Bullet> bullets_;
    std::list<Ennemy> ennemies_;
    bool Ready = false; // Booléen qui permet de savoir si la game est lancé
    std::chrono::_V2::system_clock::time_point previousTime;
    std::chrono::_V2::system_clock::time_point previousBulletBroadcastTime;
    std::chrono::_V2::system_clock::time_point previousClientBroadcastTime;

    std::thread networkThread_;
    std::atomic<bool> running_;
};
