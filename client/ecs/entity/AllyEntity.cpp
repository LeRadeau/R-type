#include "AllyEntity.hpp"
#include "ecs/component/PositionComponent.hpp"
#include "ecs/component/SpriteComponent.hpp"
#include "ecs/component/TextComponent.hpp"
#include "ecs/component/UsernameComponent.hpp"

Entity &AllyEntity::createAllyEntity(
    EntityManager &entityManager, float x, float y, const std::string &username, const sf::Font &font)
{
    Entity &ally = entityManager.createEntity();
    ally.addComponent<PositionComponent>(x, y);
    ally.addComponent<SpriteComponent>("assets/Ally64x64.png", sf::Vector2f(1, 1));
    ally.addComponent<usernameComponent>(username);
    auto &text =ally.addComponent<TextComponent>(username, font, sf::Vector2f(x, y), sf::Color::White);
    text.data.setCharacterSize(24);
    return ally;
}
