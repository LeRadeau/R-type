#pragma once

#include <SFML/Graphics/Font.hpp>
#include "ecs/EntityManager.hpp"
class AllyEntity {
  public:
    static Entity &createAllyEntity(EntityManager &entityManager, float x, float y, int health, int score,
        const std::string &username, const sf::Font &font);
};
