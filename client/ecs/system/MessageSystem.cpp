#include "MessageSystem.hpp"
#include <iostream>
#include "Serializer.hpp"
#include "ecs/component/BulletIdComponent.hpp"
#include "ecs/component/EnnemyIdComponent.hpp"
#include "ecs/component/HealthComponent.hpp"
#include "ecs/component/PositionComponent.hpp"
#include "ecs/component/RenderComponent.hpp"
#include "ecs/component/UsernameComponent.hpp"
#include "ecs/component/VelocityComponent.hpp"
#include "ecs/entity/AllyEntity.hpp"
#include "ecs/entity/BydosEntity.hpp"
#include "network_types.hpp"

MessageSystem::MessageSystem(sf::Font &font) : font_(font)
{
}

void MessageSystem::update(EntityManager &entityManager, NetworkManager &networkManager, std::string localUsername)
{
    auto receivedMessages = networkManager.getReceivedMessages();

    while (!receivedMessages.empty()) {
        auto message = receivedMessages.front();
        receivedMessages.pop();
        const char *ptr = message.data();
        auto messageType = static_cast<MessageType>(Serializer::deserialize<uint8_t>(ptr));

        switch (messageType) {
            case MessageType::UPDATE_CLIENTS: handleUpdateClients(entityManager, ptr, localUsername); break;
            case MessageType::UPDATE_BULLETS: handleUpdateBullets(entityManager, ptr); break;
            case MessageType::ERROR: handleError(ptr); break;
            case MessageType::UPDATE_ENEMIES: handleUpdateEnemies(entityManager, ptr); break;
            default: break;
        }
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
        }
    }
}

void MessageSystem::handleError(const char *&ptr)
{
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
