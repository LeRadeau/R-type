#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <unordered_map>
#include <typeindex>
#include "ecs/component/AComponent.hpp"


class Entity {
public:
    Entity() : id(nextId++) {}
    
    template<typename T, typename... Args>
    T& addComponent(Args&&... args) {
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        auto& ref = *component;
        components[std::type_index(typeid(T))] = std::move(component);
        return ref;
    }

    template<typename T>
    T* getComponent() {
        auto it = components.find(std::type_index(typeid(T)));
        return it != components.end() ? 
            dynamic_cast<T*>(it->second.get()) : nullptr;
    }

    uint64_t getId() const { return id; }

private:
    static inline uint64_t nextId = 0;
    uint64_t id;
    std::unordered_map<std::type_index, std::unique_ptr<AComponent>> components;
};

class EntityManager {
public:
    Entity& createEntity() {
        entities.push_back(std::make_unique<Entity>());
        return *entities.back();
    }

    void markForDeletion(uint64_t id) {
        toBeDeleted.push_back(id);
    }

    void destroyMarkedEntities() {
        for (uint64_t id : toBeDeleted) {
            entities.erase(
                std::remove_if(entities.begin(), entities.end(),
                    [id](const auto& e) { return e->getId() == id; }),
                entities.end()
            );
        }
        toBeDeleted.clear();
    }
    std::vector<std::unique_ptr<Entity>> entities;

private:
    std::vector<uint64_t> toBeDeleted;
};
