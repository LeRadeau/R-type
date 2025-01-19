#pragma once
#include "ecs/EntityManager.hpp"
#include "network//NetworkManager.hpp"

class MovementSystem {
  public:
    void update(EntityManager &entityManager, Network::NetworkManager &networkManager, float deltaTime, bool focus);
};
