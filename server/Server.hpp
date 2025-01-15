#pragma once

#include <SFML/Network/UdpSocket.hpp>
#include <SFML/System/Vector2.hpp>
#include <unordered_map>
#include <list>

#define SERVER_PORT 54000
#define MAX_CLIENTS 4
#define MAX_ENNEMIES 10

struct Client {
    sf::IpAddress ip;
    unsigned short port;
    std::string username;
    sf::Vector2f position;

    Client() : ip(), port(0), username(""), position(0, 0)
    {
    }
    Client(sf::IpAddress ipAddr, unsigned short p, const std::string &name)
        : ip(ipAddr), port(p), username(name), position(0, 0)
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

  private:
    void handleConnect(const sf::IpAddress &sender, unsigned short senderPort, char const *&ptr);
    void handleMove(const sf::IpAddress &sender, unsigned short senderPort, char const *&ptr);
    void handleGoodbye(const sf::IpAddress &sender, unsigned short senderPort, char const *&ptr);
    void handleShoot(const sf::IpAddress &sender, unsigned short senderPort, char const *&ptr);
    void readSocket();
    void broadcastClients();
    void broadcastBullet();
    void broadcastEnnemies();
    void updateBullets(float deltaTime);
    void updateEnnemies(float deltaTime);
    void loadEnnemies();
    void CheckEnnemyCollision();
    void enemyShoot(Ennemy &ennemy);
    void spawnEnnemies(int count);
    std::string generateBulletID(const std::string& username);
    sf::UdpSocket socket_;
    std::unordered_map<std::string, Client> clients_;
    std::list<Bullet> bullets_;
    std::list<Ennemy> ennemies_;
};
