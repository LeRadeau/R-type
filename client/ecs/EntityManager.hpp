#pragma once
#include <vector>
#include "ecs/Entity.hpp"

class EntityManager {
  public:
    Entity &createEntity();
    void markForDeletion(uint64_t id);
    void destroyMarkedEntities();

    std::vector<std::unique_ptr<Entity>> entities;

  private:
    std::vector<uint64_t> toBeDeleted;
};
