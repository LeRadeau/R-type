#include "PlayerEntity.hpp"
#include "ecs/NetworkManager.hpp"
#include "ecs/component/InputComponent.hpp"
#include "ecs/component/PositionComponent.hpp"
#include "ecs/component/SpriteComponent.hpp"
#include "ecs/component/UsernameComponent.hpp"

PlayerEntity::PlayerEntity(EntityManager &entityManager, const std::string &username, NetworkManager &networkManager)
    : entityManager_(entityManager), entity_(entityManager.createEntity())
{
    entity_.addComponent<PositionComponent>(400, 300);
    entity_.addComponent<SpriteComponent>("assets/Player64x64.png", sf::Vector2f(1, 1));
    entity_.addComponent<InputComponent>();
    entity_.addComponent<usernameComponent>(username);
    networkManager.send(MessageType::CONNECT, username);
}
