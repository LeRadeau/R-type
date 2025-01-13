#include "EventCallbacks.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "ecs/component/RectangleShapeComponent.hpp"
#include "ecs/entity/MenuEntity.hpp"
#include "ecs/entity/PlayerEntity.hpp"
#include "ecs/entity/TextFieldEntity.hpp"

namespace EventCallbacks
{
    void ButtonHandlePlay(MenuEntity &menu, Entity &entity, sf::RenderWindow &window, const sf::Event &event,
        EntityManager &entityManager, std::unique_ptr<PlayerEntity> &player, NetworkManager &networkManager)
    {
        if (!window.hasFocus())
            return;
        if (event.mouseButton.button != sf::Mouse::Left)
            return;
        auto *shape = entity.getComponent<RectangleShapeComponent>();
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        if (!shape || !shape->shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
            return;
        menu.close();
        std::string username = menu.getUsername();
        std::string ipAddress = menu.getIpAdress();
        if (ipAddress == "")
            return;
        networkManager.setRemoteIp(ipAddress);
        if (username != "" && !player)
            player = std::make_unique<PlayerEntity>(entityManager, username, networkManager);
    }

    void ButtonHandleQuit(
        EntityManager &entityManager, Entity &entity, sf::RenderWindow &window, const sf::Event &event)
    {
        if (event.mouseButton.button != sf::Mouse::Left)
            return;
        auto *shape = entity.getComponent<RectangleShapeComponent>();
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        if (!shape || !shape->shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
            return;
        for (auto &i : entityManager.entities) {
            entityManager.markForDeletion(i->getId());
        }
        window.close();
    }

    void MenuToggle(const sf::Event &event, sf::RenderWindow &window, MenuEntity &menu)
    {
        if (!window.hasFocus())
            return;
        if (event.key.code != sf::Keyboard::Escape)
            return;
        menu.toggle();
    }

    void TextFieldListener(const sf::Event &event, sf::RenderWindow &window, TextFieldEntity &textField)
    {
        if (!window.hasFocus() || !textField.isSelected())
            return;
        auto code = event.key.code;
        if (code == sf::Keyboard::LShift || code == sf::Keyboard::RShift) {
            textField.setCaps();
            return;
        }
        textField.addKey(event.key.code);
    }

    void TextFieldCapsReleaseHandler(const sf::Event &event, sf::RenderWindow &window, TextFieldEntity &textField)
    {
        if (!window.hasFocus() || !textField.isSelected())
            return;

        auto code = event.key.code;
        if (code == sf::Keyboard::LShift || code == sf::Keyboard::RShift) {
            textField.unsetCaps();
        }
    }
} // namespace EventCallbacks
