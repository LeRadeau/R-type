#pragma once
#include <vector>
#include "ecs/Entity.hpp"

class EntityManager {
  public:
    Entity &createEntity()
    {
        entities.push_back(std::make_unique<Entity>());
        return *entities.back();
    }

    void markForDeletion(uint64_t id)
    {
        toBeDeleted.push_back(id);
    }

    void destroyMarkedEntities()
    {
        for (uint64_t id : toBeDeleted) {
            entities.erase(std::remove_if(entities.begin(), entities.end(),
                               [id](const auto &e) {
                                   return e->getId() == id;
                               }),
                entities.end());
        }
        toBeDeleted.clear();
    }
    std::vector<std::unique_ptr<Entity>> entities;

  private:
    std::vector<uint64_t> toBeDeleted;
};
