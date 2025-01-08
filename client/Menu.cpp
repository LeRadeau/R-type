#include "Menu.hpp"
#include <SFML/Graphics/Color.hpp>
#include "ecs/EntityManager.hpp"
#include "ecs/component/HoverComponent.hpp"
#include "ecs/component/RectangleShapeComponent.hpp"
#include "ecs/component/SelectionComponent.hpp"
#include "ecs/component/TextComponent.hpp"

Menu::Menu(EntityManager &entityManager, const sf::Font &font) : entityManager_(entityManager), font_(font)
{
}

void Menu::createButton(const std::string &text, const sf::Vector2f &position, const sf::Vector2f &size)
{
    auto &entity = entityManager_.createEntity();
    entity.addComponent<SelectionComponent>(sf::Color(128, 128, 128), sf::Color(128, 76, 76));
    entity.addComponent<HoverComponent>(sf::Color(128, 128, 128), sf::Color(128, 128, 128, 128));
    entity.addComponent<RectangleShapeComponent>(size, position, sf::Color(128, 128, 128));
    entity.addComponent<TextComponent>(text, font_, position, sf::Color::White);
    entities_.push_back(entity.getId());
}

void Menu::open()
{
    createButton("Play", sf::Vector2f(500, 500), sf::Vector2f(150, 50));
    createButton("Quit", sf::Vector2f(660, 500), sf::Vector2f(150, 50));
}

void Menu::close()
{
    for (auto i : entities_) {
        entityManager_.markForDeletion(i);
    }
    entities_.clear();
    entityManager_.destroyMarkedEntities();
}
