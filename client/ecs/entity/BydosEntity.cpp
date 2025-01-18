#include "BydosEntity.hpp"
#include "ecs/component/EnnemyIdComponent.hpp"
#include "ecs/component/HealthComponent.hpp"
#include "ecs/component/PositionComponent.hpp"
#include "ecs/component/SpriteComponent.hpp"

Entity &BydosEntity::createBydos(EntityManager &entityManager, float x, float y, const std::string &id, int health)
{
    Entity &bydos = entityManager.createEntity();
    bydos.addComponent<PositionComponent>(x, y);
    bydos.addComponent<SpriteComponent>("assets/Bydos64x64.png", sf::Vector2f(1, 1));
    bydos.addComponent<EnnemyIdComponent>(id);
    bydos.addComponent<HealthComponent>(health);
    return bydos;
}
