#include "EventCallbacks.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "ecs/component/RectangleShapeComponent.hpp"
#include "ecs/entity/MenuEntity.hpp"
#include "ecs/entity/PlayerEntity.hpp"
#include "ecs/entity/TextFieldEntity.hpp"
#include "network/packets/PlayerConnectPacket.hpp"
#include "network/packets/PlayerReadyPacket.hpp"

namespace EventCallbacks
{
    void ButtonLaunchGame(MenuEntity &menu, Entity &entity, sf::RenderWindow &window, const sf::Event &event,
        std::unique_ptr<PlayerEntity> &player, Network::NetworkManager &networkManager)
    {
        if (!window.hasFocus())
            return;
        if (event.mouseButton.button != sf::Mouse::Left)
            return;
        auto *shape = entity.getComponent<RectangleShapeComponent>();
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        if (!shape || !shape->shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
            return;
        menu.closeLobby();
        std::string username = menu.getUsername();
        if (username != "" && !player) {
            networkManager.sendPacket(std::make_shared<Network::PlayerReadyPacket>(username));
        }
    }
    void ButtonHandlePlay(MenuEntity &menu, Entity &entity, sf::RenderWindow &window, const sf::Event &event,
        std::unique_ptr<PlayerEntity> &player, Network::NetworkManager &networkManager)
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
        try {
            networkManager.connect(ipAddress, 54000, 0);
            networkManager.start();

            if (username != "" && !player) {
                networkManager.sendPacket(
                    std::make_shared<Network::PlayerConnectPacket>(username, networkManager.getUdpPort()));
                menu.openLobby();
            }
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
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
