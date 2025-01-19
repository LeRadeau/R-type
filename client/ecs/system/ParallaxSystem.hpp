#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "ecs/EntityManager.hpp"

class ParallaxSystem {
  public:
    void update(EntityManager &entityManager, float deltaTime);

    void render(sf::RenderWindow &window, EntityManager &entityManager);
};
