#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include "ecs/EntityManager.hpp"
class EventHandlingSystem {
  public:
    void update(EntityManager &entityManager, const sf::Event &event);
};
