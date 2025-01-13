#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include "ButtonEntity.hpp"
#include "ecs/entity/PlayerEntity.hpp"
#include "ecs/entity/TextFieldEntity.hpp"

class MenuEntity {
  public:
    MenuEntity(EntityManager &entityManager, sf::RenderWindow &window, const sf::Font &font,
        std::unique_ptr<PlayerEntity> &player, NetworkManager &networkManager);

    Entity &getEntity();
    void toggle();
    void open();
    void openLobby();
    void close();
    void closeLobby();

    const std::string &getIpAdress();
    const std::string &getUsername();

  private:
    Entity &entity_;
    EntityManager &entityManager_;
    NetworkManager &networkManager_;
    sf::RenderWindow &window_;
    const sf::Font &font_;
    std::vector<std::unique_ptr<ButtonEntity>> buttons_;
    std::unique_ptr<TextFieldEntity> ipAddress_;
    std::unique_ptr<TextFieldEntity> username_;
    std::string ipAddressStr_;
    std::string usernameStr_;
    std::unique_ptr<PlayerEntity> &player_;
};
