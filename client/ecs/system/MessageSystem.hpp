#pragma once
#include "ecs/ecs.hpp"
#include "ecs/network.hpp"

class MessageSystem {
  public:
    void update(EntityManager &entityManager, NetworkManager &networkManager, std::string localUsername);
};
