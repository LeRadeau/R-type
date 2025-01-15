#include "MessageSystem.hpp"
#include <iostream>
#include "Serializer.hpp"
#include "ecs/component/BulletIdComponent.hpp"
#include "ecs/component/EnnemyIdComponent.hpp"
#include "ecs/component/HealthComponent.hpp"
#include "ecs/component/PositionComponent.hpp"
#include "ecs/component/RenderComponent.hpp"
#include "ecs/component/SoundComponent.hpp"
#include "ecs/component/UsernameComponent.hpp"
#include "ecs/component/VelocityComponent.hpp"
#include "ecs/entity/AllyEntity.hpp"
#include "ecs/entity/BydosEntity.hpp"
#include "network_types.hpp"

MessageSystem::MessageSystem(sf::Font &font) : font_(font)
{
}

void MessageSystem::update(EntityManager &entityManager, NetworkManager &networkManager, std::string localUsername, MenuEntity &menu)
{
    auto &receivedMessages = networkManager.getReceivedMessages();

    while (!receivedMessages.empty()) {
        auto message = receivedMessages.pop();
        const char *ptr = message.data();
        auto messageType = static_cast<MessageType>(Serializer::deserialize<uint8_t>(ptr));

        switch (messageType) {
            case MessageType::START_GAME: handleLaunchGame(entityManager, menu); break;
            case MessageType::WAIT: handleWaitLobby(ptr, menu); break;
            case MessageType::UPDATE_CLIENTS: handleUpdateClients(entityManager, ptr, localUsername); break;
            case MessageType::UPDATE_BULLETS: handleUpdateBullets(entityManager, ptr); break;
            case MessageType::ERROR: handleError(ptr); break;
            case MessageType::UPDATE_ENEMIES: handleUpdateEnemies(entityManager, ptr); break;
            default: break;
        }
    }
}

void MessageSystem::handleLaunchGame(EntityManager &entityManager, MenuEntity &menu)
{
    std::cout << "\033[1;32mGame has started\033[0m" << std::endl;
    menu.closeLobby();
    if (menu.getPlayer() == nullptr)
        menu.getPlayer() = std::make_unique<PlayerEntity>(entityManager, menu.getUsername(), menu.getnetworkManager());
}

void MessageSystem::handleWaitLobby(const char *&ptr, MenuEntity &menu)
{
    auto nbrClients = Serializer::deserialize<std::size_t>(ptr);
    std::cout << "\033[1;33mCurrentyly there is -> " << nbrClients << " clients in the game" <<"\033[0m"<< std::endl;
    if (nbrClients != menu.getNbrClients()) {
        menu.setNbrClients(nbrClients);
    }
}

void MessageSystem::handleUpdateClients(
    EntityManager &entityManager, const char *&ptr, const std::string &localUsername)
{
    auto numClients = Serializer::deserialize<uint32_t>(ptr);
    for (uint32_t i = 0; i < numClients; ++i) {
        auto username = Serializer::deserializeString(ptr);
        auto x = Serializer::deserialize<float>(ptr);
        auto y = Serializer::deserialize<float>(ptr);
        Entity *clientEntity = nullptr;

        for (auto &entity : entityManager.entities) {
            auto *usernameComp = entity->getComponent<usernameComponent>();
            if (usernameComp && usernameComp->username == username) {
                clientEntity = entity.get();
                break;
            }
        }
        if (clientEntity) {
            auto *position = clientEntity->getComponent<PositionComponent>();
            if (position && localUsername != username) {
                position->position.x = x;
                position->position.y = y;
            }
        } else {
            AllyEntity::createAllyEntity(entityManager, x, y, username, font_);
        }
    }
}

void MessageSystem::handleUpdateBullets(EntityManager &entityManager, const char *&ptr)
{

    auto numBullets = Serializer::deserialize<uint32_t>(ptr);
    for (uint32_t i = 0; i < numBullets; ++i) {
        auto id = Serializer::deserializeString(ptr);
        float x = Serializer::deserialize<float>(ptr);
        float y = Serializer::deserialize<float>(ptr);
        float vx = Serializer::deserialize<float>(ptr);
        float vy = Serializer::deserialize<float>(ptr);

        Entity *bulletEntity = nullptr;

        for (auto &entity : entityManager.entities) {
            auto *bulletId = entity->getComponent<BulletIdComponent>();
            if (bulletId && bulletId->id == id) {
                bulletEntity = entity.get();
                break;
            }
        }

        if (!bulletEntity) {
            auto &newEntity = entityManager.createEntity();
            newEntity.addComponent<PositionComponent>(x, y);
            newEntity.addComponent<RenderComponent>(5, sf::Color::Red);
            newEntity.addComponent<BulletIdComponent>(id);
            newEntity.addComponent<VelocityComponent>(vx, vy);
            newEntity.addComponent<SoundComponent>("assets/shoot.ogg", 1);
        }
    }
}

void MessageSystem::handleError(const char *&ptr)
{
    // Segfault problem
    std::cerr << "Received error message from server: " << Serializer::deserializeString(ptr) << std::endl;
}

void MessageSystem::handleUpdateEnemies(EntityManager &entityManager, const char *&ptr)
{
    auto numEnemies = Serializer::deserialize<uint32_t>(ptr);
    for (uint32_t i = 0; i < numEnemies; ++i) {
        auto id = Serializer::deserializeString(ptr);
        float x = Serializer::deserialize<float>(ptr);
        float y = Serializer::deserialize<float>(ptr);
        int health = Serializer::deserialize<int>(ptr);

        Entity *enemyEntity = nullptr;

        for (auto &entity : entityManager.entities) {
            auto *enemyId = entity->getComponent<EnnemyIdComponent>();
            if (enemyId && enemyId->id == id) {
                enemyEntity = entity.get();
                break;
            }
        }

        if (enemyEntity) {
            auto *position = enemyEntity->getComponent<PositionComponent>();
            auto *healthComp = enemyEntity->getComponent<HealthComponent>();
            if (position) {
                position->position.x = x;
                position->position.y = y;
            }
            if (healthComp) {
                healthComp->health = health;
            }
        }

        if (!enemyEntity) {
            BydosEntity::createBydos(entityManager, x, y, id, health);
        }
    }
}
