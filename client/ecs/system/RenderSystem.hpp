#pragma once
#include <SFML/Graphics.hpp>
#include "ecs/ecs.hpp"

class RenderSystem {
  public:
    RenderSystem(sf::RenderWindow &window, const std::string &fontName = "assets/arial.ttf");

    void displayBarInfos(EntityManager &entityManager);

    void update(EntityManager &entityManager);

  private:
    sf::RenderWindow &window;
    sf::Font font;
};
