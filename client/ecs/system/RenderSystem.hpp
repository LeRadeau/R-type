#pragma once
#include <SFML/Graphics.hpp>
#include "ecs/EntityManager.hpp"

class RenderSystem {
  public:
    RenderSystem(sf::RenderWindow &window, const std::string &fontName = "assets/arial.ttf");

    void update(EntityManager &entityManager, const std::string &playerUsername);

  private:
    sf::RenderWindow &window;
    sf::Font font;
};
