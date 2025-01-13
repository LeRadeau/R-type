#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "ecs/Entity.hpp"
#include "ecs/EntityManager.hpp"
class TextFieldEntity {
  public:
    TextFieldEntity(EntityManager &entityManager, sf::RenderWindow &window, sf::Vector2f size, sf::Vector2f position,
        const sf::Font &font, const std::string &title);

    bool isSelected() const;
    void addKey(sf::Keyboard::Key key);
    std::string getFieldValue() const;
    void setFieldValue(const std::string &value);
    void setCaps();
    void unsetCaps();
    Entity &getEntity();
    Entity &getTitleEntity();

  private:
    Entity &entity_;
    Entity &titleEntity_;
    EntityManager &entityManager_;
    sf::RenderWindow &window_;
    char conversion_;
};
