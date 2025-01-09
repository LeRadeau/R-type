#pragma once
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Vector2.hpp>
#include "ecs/EntityManager.hpp"

class MenuSystem {
  public:
    MenuSystem(EntityManager &entityManager, const sf::Font &font);
    void toggle();
    void open();
    void close();
    void update();

  private:
    void createButton(const std::string &text, const sf::Vector2f &position, const sf::Vector2f &size);
    EntityManager &entityManager_;
    const sf::Font &font_;
    std::vector<uint64_t> entities_;
};
