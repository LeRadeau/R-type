#pragma once
#include <SFML/Graphics/Font.hpp>
#include "ecs/EntityManager.hpp"
#include "ecs/NetworkManager.hpp"

class MessageSystem {
  public:
    MessageSystem(sf::Font &font);
    void update(EntityManager &entityManager, NetworkManager &networkManager, std::string localUsername);

  private:
    void handleUpdateClients(EntityManager &entityManager, const char *&ptr, const std::string &localUsername);
    void handleUpdateBullets(EntityManager &entityManager, const char *&ptr);
    void handleError(const char *&ptr);
    void handleUpdateEnemies(EntityManager &entityManager, const char *&ptr);
    sf::Font &font_;
};
