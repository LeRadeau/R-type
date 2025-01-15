#include "HudEntity.hpp"
#include "ecs/component/RectangleShapeComponent.hpp"

Entity &HudEntity::createHudEntity(EntityManager &entityManager)
{
  Entity &hud = entityManager.createEntity();
  hud.addComponent<RectangleShapeComponent>(sf::Vector2f(1920, 50), sf::Vector2f(0, 0), sf::Color(0, 0, 255, 50));
  return hud;
}
