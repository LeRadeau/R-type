#pragma once
#include "ecs/EntityManager.hpp"
#include "ecs/NetworkManager.hpp"

class MessageSystem {
  public:
    void update(EntityManager &entityManager, NetworkManager &networkManager, std::string localUsername);
};
