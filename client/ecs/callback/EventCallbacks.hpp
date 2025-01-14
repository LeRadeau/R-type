#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "ecs/EntityManager.hpp"
#include "ecs/entity/MenuEntity.hpp"
#include "ecs/entity/TextFieldEntity.hpp"

namespace EventCallbacks
{
    void ButtonHandlePlay(MenuEntity &menu, Entity &entity, sf::RenderWindow &window, const sf::Event &event,
        std::unique_ptr<PlayerEntity> &player, NetworkManager &networkManager);
    void ButtonLaunchGame(MenuEntity &menu, Entity &entity, sf::RenderWindow &window, const sf::Event &event,
        std::unique_ptr<PlayerEntity> &player, NetworkManager &networkManager);
    void ButtonHandleQuit(
        EntityManager &entityManager, Entity &entity, sf::RenderWindow &window, const sf::Event &event);

    void MenuToggle(const sf::Event &event, sf::RenderWindow &window, MenuEntity &menu);
    void TextFieldListener(const sf::Event &event, sf::RenderWindow &window, TextFieldEntity &textField);
    void TextFieldCapsReleaseHandler(const sf::Event &event, sf::RenderWindow &window, TextFieldEntity &textField);
} // namespace EventCallbacks
