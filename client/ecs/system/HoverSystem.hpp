#pragma once
#include "ecs/EntityManager.hpp"

class HoverSystem {
  public:
    void update(EntityManager &entityManager, float x, float y);
};
