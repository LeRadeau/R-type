
#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

#include "../Components/Component.hpp"

using Entity = std::size_t;

class EntityManager {
private:
    std::unordered_map<Entity, std::unordered_map<std::string, std::shared_ptr<Component>>> components;
    Entity nextEntity = 0;

public:
    Entity createEntity() {
        return nextEntity++;
    }

    template<typename T, typename... Args>
    void addComponent(Entity entity, const std::string& name, Args&&... args) {
        components[entity][name] = std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    std::shared_ptr<T> getComponent(Entity entity, const std::string& name) {
        return std::static_pointer_cast<T>(components[entity][name]);
    }

    template<typename T>
    std::vector<Entity> getEntitiesWithComponent(const std::string& name) {
        std::vector<Entity> result;
        for (const auto& [entity, compMap] : components) {
            if (compMap.find(name) != compMap.end()) {
                result.push_back(entity);
            }
        }
        return result;
    }
};