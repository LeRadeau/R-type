#include "HudEntity.hpp"
#include "ecs/component/RectangleShapeComponent.hpp"

Entity &HudEntity::createHudEntity(EntityManager &entityManager, const std::string &font)
{
  Entity &hud = entityManager.createEntity();
  hud.addComponent<RectangleShapeComponent>(sf::Vector2f(400, 300), sf::Vector2f(0, 0), sf::Color(255, 255, 255, 50));
  return hud;
}
