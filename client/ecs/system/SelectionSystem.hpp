#pragma once

#include <SFML/Window/Event.hpp>
#include "ecs/EntityManager.hpp"

class SelectionSystem {
  public:
    void update(EntityManager &entityManager, const sf::Event::MouseButtonEvent &mouseEvent);
};
