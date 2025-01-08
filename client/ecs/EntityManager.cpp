#include "EntityManager.hpp"

Entity &EntityManager::createEntity()
{
    entities.push_back(std::make_unique<Entity>());
    return *entities.back();
}

void EntityManager::markForDeletion(uint64_t id)
{
    toBeDeleted.push_back(id);
}

void EntityManager::destroyMarkedEntities()
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
