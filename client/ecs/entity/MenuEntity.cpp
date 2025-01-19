#include "MenuEntity.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include "ecs/EntityManager.hpp"
#include "ecs/callback/EventCallbacks.hpp"
#include "ecs/callback/NetworkCallbacks.hpp"
#include "ecs/component/NetworkCallbackComponent.hpp"
#include "ecs/component/TextComponent.hpp"
#include "network/packets/Packet.hpp"

MenuEntity::MenuEntity(EntityManager &entityManager, sf::RenderWindow &window, const sf::Font &font,
    std::unique_ptr<PlayerEntity> &player, Network::NetworkManager &networkManager)
    : entity_(entityManager.createEntity()), entityManager_(entityManager), networkManager_(networkManager),
      window_(window), font_(font), player_(player), entityText_(entityManager.createEntity())
{
    open();
    entity_.addComponent<EventHandlerComponent>(sf::Event::KeyReleased, [this, &window](const sf::Event &event) {
        EventCallbacks::MenuToggle(event, window, *this);
    });
}

Entity &MenuEntity::getEntity()
{
    return entity_;
}

void MenuEntity::toggle()
{
    if (buttons_.size() == 0) {
        open();
    } else {
        close();
    }
}
void MenuEntity::openLobby()
{
    sf::Vector2f size(275, 50);
    sf::Vector2f position(window_.getSize().x / 2.0f - 300, 500);
    sf::Vector2f positionText(position.x, position.y - 100);

    entityText_.addComponent<TextComponent>("?/4 players", font_, positionText, sf::Color::White);
    auto &comp = entityText_.addComponent<NetworkCallbackComponent>();
    comp.setCallback(
        Network::Packet::PacketType::WAITING_UPDATE, [this](const std::shared_ptr<Network::Packet> &packet) {
            NetworkCallbacks::onWaitUpdateClientNbr(packet, entityText_);
        });
    comp.setCallback(Network::Packet::PacketType::GAME_START, [this](const std::shared_ptr<Network::Packet> &) {
        closeLobby();
    });
    buttons_.push_back(std::make_unique<ButtonEntity>(entityManager_, size, position, "LANCER LA PARTIE", font_));
    buttons_[0]->setCallback(sf::Event::MouseButtonReleased, [this](const sf::Event &event) {
        EventCallbacks::ButtonLaunchGame(*this, buttons_[0]->getEntity(), window_, event, player_, networkManager_);
    });
}

void MenuEntity::closeLobby()
{
    for (auto &i : buttons_) {
        entityManager_.markForDeletion(i->getEntity().getId());
    }
    buttons_.clear();
    entityManager_.markForDeletion(entityText_.getId());
}

void MenuEntity::open()
{
    sf::Vector2f size(150, 50);
    sf::Vector2f position(window_.getSize().x / 2.0f - 300, 500);
    buttons_.push_back(std::make_unique<ButtonEntity>(entityManager_, size, position, "Play", font_));
    buttons_[0]->setCallback(sf::Event::MouseButtonReleased, [this](const sf::Event &event) {
        EventCallbacks::ButtonHandlePlay(*this, buttons_[0]->getEntity(), window_, event, player_, networkManager_);
    });
    position.x += 160;
    buttons_.push_back(std::make_unique<ButtonEntity>(entityManager_, size, position, "Quit", font_));
    buttons_[1]->setCallback(sf::Event::MouseButtonReleased, [this](const sf::Event &event) {
        EventCallbacks::ButtonHandleQuit(entityManager_, buttons_[1]->getEntity(), window_, event);
    });
    position.x = window_.getSize().x / 2.0f - 300;
    position.y = 300;
    size.x = 250;
    ipAddress_ = std::make_unique<TextFieldEntity>(entityManager_, window_, size, position, font_, "IP :");
    position.x += 260;
    username_ = std::make_unique<TextFieldEntity>(entityManager_, window_, size, position, font_, "Username :");
    ipAddress_->setFieldValue(ipAddressStr_);
    username_->setFieldValue(usernameStr_);
}

void MenuEntity::close()
{
    ipAddressStr_ = ipAddress_->getFieldValue();
    usernameStr_ = username_->getFieldValue();
    for (auto &i : buttons_) {
        entityManager_.markForDeletion(i->getEntity().getId());
    }
    buttons_.clear();
    entityManager_.markForDeletion(ipAddress_->getEntity().getId());
    entityManager_.markForDeletion(ipAddress_->getTitleEntity().getId());
    ipAddress_.reset();
    entityManager_.markForDeletion(username_->getEntity().getId());
    entityManager_.markForDeletion(username_->getTitleEntity().getId());
    username_.reset();
    entityManager_.destroyMarkedEntities();
}

const std::string &MenuEntity::getIpAdress()
{
    return ipAddressStr_;
}

const std::string &MenuEntity::getUsername()
{
    return usernameStr_;
}
