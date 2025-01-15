#pragma once

#include "ecs/EntityManager.hpp"
class SoundSystem {
  public:
    void update(EntityManager &entityManager, int volume);
};
