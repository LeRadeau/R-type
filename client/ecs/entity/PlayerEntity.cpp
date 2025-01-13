#include "PlayerEntity.hpp"
#include "ecs/NetworkManager.hpp"
#include "ecs/component/InputComponent.hpp"
#include "ecs/component/PositionComponent.hpp"
#include "ecs/component/RenderComponent.hpp"
#include "ecs/component/SoundComponent.hpp"
#include "ecs/component/UsernameComponent.hpp"

PlayerEntity::PlayerEntity(EntityManager &entityManager, const std::string &username, NetworkManager &networkManager)
    : entityManager_(entityManager), entity_(entityManager.createEntity())
{
    entity_.addComponent<PositionComponent>(400, 300);
    entity_.addComponent<RenderComponent>(30, sf::Color::Green);
    entity_.addComponent<InputComponent>();
    entity_.addComponent<usernameComponent>(username);
    auto &sound = entity_.addComponent<SoundComponent>("assets/spaceshipIdle.mp3", -1);
    sound.sound.setPitch(0.5);
    networkManager.send(MessageType::CONNECT, username);
}
