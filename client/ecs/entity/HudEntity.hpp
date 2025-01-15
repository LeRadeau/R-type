#pragma once

#include "ecs/Entity.hpp"
#include "ecs/EntityManager.hpp"

class HudEntity {
  public:
    static Entity &createHudEntity(EntityManager &entityManager);
};
