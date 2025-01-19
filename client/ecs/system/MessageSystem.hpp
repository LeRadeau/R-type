#pragma once
#include <SFML/Graphics/Font.hpp>
#include "ecs/EntityManager.hpp"
#include "ecs/entity/PlayerEntity.hpp"
#include "network/NetworkManager.hpp"

class MessageSystem {
  public:
    void update(EntityManager &entityManager, Network::NetworkManager &networkManager, std::string localUsername,
        std::unique_ptr<PlayerEntity> &player);
    MessageSystem(sf::Font &font);

  private:
    void handleLaunchGame(
        EntityManager &entityManager, std::unique_ptr<PlayerEntity> &player, const std::string &username);
    void handleUpdateClients(
        EntityManager &entityManager, const std::shared_ptr<Network::Packet> &, const std::string &localUsername);
    void handleUpdateBullets(EntityManager &entityManager, const std::shared_ptr<Network::Packet> &);
    void handleError(const std::shared_ptr<Network::Packet> &);
    void handleUpdateEnemies(EntityManager &entityManager, const std::shared_ptr<Network::Packet> &);
    void handleBulletHit(EntityManager &entityManager, const std::shared_ptr<Network::Packet> &);
    void handleEnemyDeath(EntityManager &entityManager, const std::shared_ptr<Network::Packet> &);
    void handlePlayerDeath(EntityManager &entityManager, const std::shared_ptr<Network::Packet> &);
    void handleGameOver(EntityManager &entityManager);
    sf::Font &font_;
};
