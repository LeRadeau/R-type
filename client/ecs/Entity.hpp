#pragma once
#include <algorithm>
#include <memory>
#include <typeindex>
#include "ecs/component/AComponent.hpp"
#include <unordered_map>
class Entity {
  public:
    Entity();

    template <typename T, typename... Args> T &addComponent(Args &&...args)
    {
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        auto &ref = *component;
        components[std::type_index(typeid(T))] = std::move(component);
        return ref;
    }

    template <typename T> T *getComponent()
    {
        auto it = components.find(std::type_index(typeid(T)));
        return it != components.end() ? dynamic_cast<T *>(it->second.get()) : nullptr;
    }

    uint64_t getId() const;

  private:
    static inline uint64_t nextId = 0;
    uint64_t id;
    std::unordered_map<std::type_index, std::unique_ptr<AComponent>> components;
};
