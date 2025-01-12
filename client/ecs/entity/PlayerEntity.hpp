#pragma once

#include "ecs/EntityManager.hpp"
#include "ecs/NetworkManager.hpp"
class PlayerEntity {
  public:
    PlayerEntity(EntityManager &entityManager, const std::string &username, NetworkManager &networkManager);

  private:
    EntityManager &entityManager_;
    Entity &entity_;
};
