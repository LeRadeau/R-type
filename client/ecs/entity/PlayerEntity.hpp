#pragma once

#include "ecs/EntityManager.hpp"
class PlayerEntity {
  public:
    PlayerEntity(EntityManager &entityManager, const std::string &username);

  private:
    Entity &entity_;
};
