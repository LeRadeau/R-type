#pragma once

#include "ecs/EntityManager.hpp"
#include "ecs/component/ParallaxComponent.hpp"
#include "SFML/Graphics.hpp"

class ParallaxSystem {
public:
    void update(EntityManager &entityManager, float deltaTime);

    void render(sf::RenderWindow &window, EntityManager &entityManager);
};
