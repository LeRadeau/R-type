#include "MessageSystem.hpp"
#include <iostream>
#include "Serializer.hpp"
#include "ecs/component/BulletIdComponent.hpp"
#include "ecs/component/EnnemyIdComponent.hpp"
#include "ecs/component/HealthComponent.hpp"
#include "ecs/component/NetworkCallbackComponent.hpp"
#include "ecs/component/PositionComponent.hpp"
#include "ecs/component/RenderComponent.hpp"
#include "ecs/component/ScoreComponent.hpp"
#include "ecs/component/SoundComponent.hpp"
#include "ecs/component/TextComponent.hpp"
#include "ecs/component/UsernameComponent.hpp"
#include "ecs/component/VelocityComponent.hpp"
#include "ecs/entity/AllyEntity.hpp"
#include "ecs/entity/BydosEntity.hpp"
#include "ecs/entity/PlayerEntity.hpp"
#include "ecs/entity/HudEntity.hpp"
#include "network_types.hpp"

MessageSystem::MessageSystem(sf::Font &font) : font_(font)
{
}

void MessageSystem::update(EntityManager &entityManager, NetworkManager &networkManager, std::string localUsername,
    std::unique_ptr<PlayerEntity> &player)
{
    auto &receivedMessages = networkManager.getReceivedMessages();

    while (!receivedMessages.empty()) {
        auto message = receivedMessages.pop();
        const char *ptr = message.data();
        auto messageType = static_cast<MessageType>(Serializer::deserialize<uint8_t>(ptr));
        // Check for callbacks loop
        for (size_t i = 0; i < entityManager.entities.size(); i++) {
            auto &currentEntity = entityManager.entities[i];
            auto *networkCallbackComponent = currentEntity->getComponent<NetworkCallbackComponent>();
            if (!networkCallbackComponent
                || networkCallbackComponent->callbacks.find(messageType) == networkCallbackComponent->callbacks.end()) {
                continue;
            }
            networkCallbackComponent->callbacks.at(messageType)(ptr);
        }
        switch (messageType) {
            case MessageType::START_GAME: handleLaunchGame(entityManager, player, localUsername); break;
            case MessageType::UPDATE_CLIENTS: handleUpdateClients(entityManager, ptr, localUsername); break;
            case MessageType::UPDATE_BULLETS: handleUpdateBullets(entityManager, ptr); break;
            case MessageType::ERROR: handleError(ptr); break;
            case MessageType::UPDATE_ENEMIES: handleUpdateEnemies(entityManager, ptr); break;
            case MessageType::BULLET_HIT: handleBulletHit(entityManager, ptr); break;
            case MessageType::ENEMY_DEATH: handleEnemyDeath(entityManager, ptr); break;
            case MessageType::PLAYER_DEATH: handlePlayerDeath(entityManager, ptr); break;
            case MessageType::GAME_OVER: handleGameOver(entityManager); break;
            default: break;
        }
    }
}

void MessageSystem::handleLaunchGame(
    EntityManager &entityManager, std::unique_ptr<PlayerEntity> &player, const std::string &username)
{
    std::cout << "\033[1;32mGame has started\033[0m" << std::endl;
    if (player == nullptr)
        player = std::make_unique<PlayerEntity>(entityManager, username);
    HudEntity::createHudEntity(entityManager, "assets/arial.ttf");
}

void MessageSystem::handleUpdateClients(
    EntityManager &entityManager, const char *&ptr, const std::string &localUsername)
{
    auto numClients = Serializer::deserialize<uint32_t>(ptr);
    for (uint32_t i = 0; i < numClients; ++i) {
        auto username = Serializer::deserializeString(ptr);
        auto x = Serializer::deserialize<float>(ptr);
        auto y = Serializer::deserialize<float>(ptr);
        auto health = Serializer::deserialize<int>(ptr);
        auto score = Serializer::deserialize<int>(ptr);
        Entity *clientEntity = nullptr;

        for (auto &entity : entityManager.entities) {
            auto *usernameComp = entity->getComponent<UsernameComponent>();
            if (usernameComp && usernameComp->username == username) {
                clientEntity = entity.get();
                break;
            }
        }
        if (clientEntity) {
            auto *position = clientEntity->getComponent<PositionComponent>();
            auto *healthComponent = clientEntity->getComponent<HealthComponent>();
            auto *scoreComponent = clientEntity->getComponent<ScoreComponent>();
            if (scoreComponent)
                scoreComponent->score = score;
            if (healthComponent)
                healthComponent->health = health;
            if (position && localUsername != username) {
                position->position.x = x;
                position->position.y = y;
            }
        } else {
            AllyEntity::createAllyEntity(entityManager, x, y, health, score, username, font_);
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

void MessageSystem::handleEnemyDeath(EntityManager &entityManager, const char *&ptr)
{
    auto entity = entityManager.entities.begin();
    std::string enemyId = Serializer::deserializeString(ptr);

    while (entity != entityManager.entities.end()) {
        auto *enemyIdComponent = entity->get()->getComponent<EnnemyIdComponent>();

        if (enemyIdComponent && enemyIdComponent->id == enemyId) {
            entityManager.markForDeletion(entity->get()->getId());
        }
        entity++;
    }
    entityManager.destroyMarkedEntities();
}

void MessageSystem::handlePlayerDeath(EntityManager &entityManager, const char *&ptr)
{
    auto entity = entityManager.entities.begin();
    std::string username = Serializer::deserializeString(ptr);

    while (entity != entityManager.entities.end()) {
        auto *usernameComponent = entity->get()->getComponent<UsernameComponent>();

        if (usernameComponent && usernameComponent->username == username) {
            entityManager.markForDeletion(entity->get()->getId());
        }
        entity++;
    }
}

void MessageSystem::handleGameOver(EntityManager &entityManager)
{
    entityManager.entities.clear();
    auto &entity = entityManager.createEntity();

    entity.addComponent<TextComponent>("GAME OVER", font_, sf::Vector2f(500, 500), sf::Color::White)
        .data.setCharacterSize(40);
}

void MessageSystem::handleBulletHit(EntityManager &entityManager, const char *&ptr)
{
    auto entity = entityManager.entities.begin();
    std::string bulletId = Serializer::deserializeString(ptr);

    while (entity != entityManager.entities.end()) {
        auto *bulletIdComponent = entity->get()->getComponent<BulletIdComponent>();
        if (bulletIdComponent && bulletIdComponent->id == bulletId) {
            entity = entityManager.entities.erase(entity);
        } else {
            entity++;
        }
    }
}
