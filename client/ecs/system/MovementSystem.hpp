#pragma once
#include "ecs/EntityManager.hpp"
#include "ecs/NetworkManager.hpp"

class MovementSystem {
  public:
    void update(EntityManager &entityManager, NetworkManager &networkManager, float deltaTime, bool focus);
};
