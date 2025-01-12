#include "EventHandlingSystem.hpp"
#include <iostream>
#include "ecs/component/EventHandlerComponent.hpp"

void EventHandlingSystem::update(EntityManager &entityManager, const sf::Event &event)
{
    for (size_t i = 0; i < entityManager.entities.size(); i++) {
        auto &entity = entityManager.entities[i];
        auto *eventHandlerComponent = entity->getComponent<EventHandlerComponent>();

        if (eventHandlerComponent) {
            if (eventHandlerComponent->callbacks.find(event.type) != eventHandlerComponent->callbacks.end())
                try {
                    eventHandlerComponent->callbacks.at(event.type)(event);
                } catch (std::exception &e) {
                    std::cerr << e.what() << std::endl;
                }
        }
    }
    entityManager.destroyMarkedEntities();
}
