#pragma once

#include "ecs/Entity.hpp"
#include "ecs/EntityManager.hpp"
class BydosEntity {
  public:
    static Entity &createBydos(EntityManager &entityManager, float x, float y, const std::string &id, int health);
};
