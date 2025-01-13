#pragma once
#include "ecs/EntityManager.hpp"
#include "ecs/NetworkManager.hpp"
#include "ecs/entity/MenuEntity.hpp"

class MessageSystem {
  public:
    void update(EntityManager &entityManager, NetworkManager &networkManager, std::string localUsername, MenuEntity &menu);
  private:
    void handleLaunchGame(EntityManager &entityManager, const char *&ptr, MenuEntity& menu);
    void handleWaitLobby(EntityManager &entityManager, const char *&ptr);
    void handleUpdateClients(EntityManager &entityManager, const char *&ptr, const std::string &localUsername);
    void handleUpdateBullets(EntityManager &entityManager, const char *&ptr);
    void handleError(const char *&ptr);
    void handleUpdateEnemies(EntityManager &entityManager, const char *&ptr);
};
