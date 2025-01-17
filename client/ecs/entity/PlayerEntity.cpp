#include "PlayerEntity.hpp"
#include "ecs/component/HealthComponent.hpp"
#include "ecs/component/InputComponent.hpp"
#include "ecs/component/PositionComponent.hpp"
#include "ecs/component/ScoreComponent.hpp"
#include "ecs/component/SoundComponent.hpp"
#include "ecs/component/SpriteComponent.hpp"
#include "ecs/component/UsernameComponent.hpp"

PlayerEntity::PlayerEntity(EntityManager &entityManager, const std::string &username)
    : entity_(entityManager.createEntity())
{
    entity_.addComponent<PositionComponent>(400, 300);
    entity_.addComponent<SpriteComponent>("assets/Player64x64.png", sf::Vector2f(1, 1));
    entity_.addComponent<InputComponent>();
    entity_.addComponent<UsernameComponent>(username);
    auto &sound = entity_.addComponent<SoundComponent>("assets/spaceshipIdle.mp3", -1);
    sound.sound.setPitch(0.5);
    entity_.addComponent<ScoreComponent>();
    entity_.addComponent<HealthComponent>();
}
