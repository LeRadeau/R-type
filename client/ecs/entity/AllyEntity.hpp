#pragma once

#include <SFML/Graphics/Font.hpp>
#include "ecs/EntityManager.hpp"
class AllyEntity {
  public:
    static Entity &createAllyEntity(
        EntityManager &entityManager, float x, float y, const std::string &username, const sf::Font &font);
};
