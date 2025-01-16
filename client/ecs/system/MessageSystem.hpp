#pragma once
#include <SFML/Graphics/Font.hpp>
#include "ecs/EntityManager.hpp"
#include "ecs/NetworkManager.hpp"
#include "ecs/entity/PlayerEntity.hpp"

class MessageSystem {
  public:
    void update(EntityManager &entityManager, NetworkManager &networkManager, std::string localUsername,
        std::unique_ptr<PlayerEntity> &player);
    MessageSystem(sf::Font &font);

  private:
    void handleLaunchGame(
        EntityManager &entityManager, std::unique_ptr<PlayerEntity> &player, const std::string &username);
    void handleUpdateClients(EntityManager &entityManager, const char *&ptr, const std::string &localUsername);
    void handleUpdateBullets(EntityManager &entityManager, const char *&ptr);
    void handleError(const char *&ptr);
    void handleUpdateEnemies(EntityManager &entityManager, const char *&ptr);
    void handleBulletHit(EntityManager &entityManager, const char *&ptr);
    void handleEnemyDeath(EntityManager &entityManager, const char *&ptr);
    void handlePlayerDeath(EntityManager &entityManager, const char *&ptr);
    void handleGameOver(EntityManager &entityManager);
    sf::Font &font_;
};
