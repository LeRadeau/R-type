#pragma once

#include <SFML/Graphics.hpp>
#include "ecs/Entity.hpp"
#include "ecs/EntityManager.hpp"

class HudEntity {
  public:
    static Entity &createHudEntity(EntityManager &entityManager, const std::string &font);
};
