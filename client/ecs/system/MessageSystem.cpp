#include "MessageSystem.hpp"
#include <iostream>
#include <memory>
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
#include "network/NetworkManager.hpp"
#include "network/packets/BulletHitPacket.hpp"
#include "network/packets/BulletsUpdatePacket.hpp"
#include "network/packets/EnemiesUpdatePacket.hpp"
#include "network/packets/EnemyDeathPacket.hpp"
#include "network/packets/ErrorPacket.hpp"
#include "network/packets/PlayerDeathPacket.hpp"
#include "network/packets/PlayersUpdatePacket.hpp"

MessageSystem::MessageSystem(sf::Font &font) : font_(font)
{
}

void MessageSystem::update(EntityManager &entityManager, Network::NetworkManager &networkManager,
    std::string localUsername, std::unique_ptr<PlayerEntity> &player)
{
    if (!networkManager.isRunning())
        return;
    auto packet = networkManager.getNextPacket();

    while (packet.has_value()) {
        auto message = packet.value();
        auto messageType = message.packet->getType();
        // Check for callbacks loop
        for (size_t i = 0; i < entityManager.entities.size(); i++) {
            auto &currentEntity = entityManager.entities[i];
            auto *networkCallbackComponent = currentEntity->getComponent<NetworkCallbackComponent>();
            if (!networkCallbackComponent
                || networkCallbackComponent->callbacks.find(messageType) == networkCallbackComponent->callbacks.end()) {
                continue;
            }
            networkCallbackComponent->callbacks.at(messageType)(message.packet);
        }
        switch (messageType) {
            case Network::Packet::PacketType::GAME_START: handleLaunchGame(entityManager, player, localUsername); break;
            case Network::Packet::PacketType::PLAYERS_UPDATE:
                handleUpdateClients(entityManager, packet->packet, localUsername);
                break;
            case Network::Packet::PacketType::BULLETS_UPDATE: handleUpdateBullets(entityManager, packet->packet); break;
            case Network::Packet::PacketType::ERROR: handleError(packet->packet); break;
            case Network::Packet::PacketType::ENEMIES_UPDATE: handleUpdateEnemies(entityManager, packet->packet); break;
            case Network::Packet::PacketType::BULLET_HIT: handleBulletHit(entityManager, packet->packet); break;
            case Network::Packet::PacketType::ENEMY_DEATH: handleEnemyDeath(entityManager, packet->packet); break;
            case Network::Packet::PacketType::PLAYER_DEATH: handlePlayerDeath(entityManager, packet->packet); break;
            case Network::Packet::PacketType::GAME_OVER: handleGameOver(entityManager); break;
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
}

void MessageSystem::handleUpdateClients(
    EntityManager &entityManager, const std::shared_ptr<Network::Packet> &packet, const std::string &localUsername)
{
    auto updatePacket = std::dynamic_pointer_cast<Network::PlayersUpdatePacket>(packet);

    if (!updatePacket)
        return;
    const auto &data = updatePacket->getData();
    for (uint32_t i = 0; i < data.size(); ++i) {
        auto username = data[i].username;
        auto x = data[i].x;
        auto y = data[i].y;
        auto health = data[i].health;
        auto score = data[i].score;

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

void MessageSystem::handleUpdateBullets(EntityManager &entityManager, const std::shared_ptr<Network::Packet> &packet)
{
    auto updatePacket = std::dynamic_pointer_cast<Network::BulletsUpdatePacket>(packet);

    if (!updatePacket)
        return;
    const auto &data = updatePacket->getData();
    for (uint32_t i = 0; i < data.size(); ++i) {
        auto id = data[i].id;
        float x = data[i].x;
        float y = data[i].y;
        float vx = data[i].vx;
        float vy = data[i].vy;

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

void MessageSystem::handleError(const std::shared_ptr<Network::Packet> &packet)
{
    auto errorPacket = std::dynamic_pointer_cast<Network::ErrorPacket>(packet);
    if (!errorPacket)
        return;
    const auto &data = errorPacket->getErrorMessage();
    std::cerr << "Received error message from server: " << data << std::endl;
}

void MessageSystem::handleUpdateEnemies(EntityManager &entityManager, const std::shared_ptr<Network::Packet> &packet)
{
    auto updatePacket = std::dynamic_pointer_cast<Network::EnemiesUpdatePacket>(packet);

    if (!updatePacket)
        return;
    const auto &data = updatePacket->getData();
    for (uint32_t i = 0; i < data.size(); ++i) {
        auto id = data[i].id;
        float x = data[i].x;
        float y = data[i].y;
        int health = data[i].health;

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

void MessageSystem::handleEnemyDeath(EntityManager &entityManager, const std::shared_ptr<Network::Packet> &packet)
{
    auto enemyDeathPacket = std::dynamic_pointer_cast<Network::EnemyDeathPacket>(packet);

    if (!enemyDeathPacket)
        return;

    auto enemyId = enemyDeathPacket->getEnemyId();
    auto entity = entityManager.entities.begin();
    while (entity != entityManager.entities.end()) {
        auto *enemyIdComponent = entity->get()->getComponent<EnnemyIdComponent>();

        if (enemyIdComponent && enemyIdComponent->id == enemyId) {
            entityManager.markForDeletion(entity->get()->getId());
        }
        entity++;
    }
    entityManager.destroyMarkedEntities();
}

void MessageSystem::handlePlayerDeath(EntityManager &entityManager, const std::shared_ptr<Network::Packet> &packet)
{
    auto playerDeathPacket = std::dynamic_pointer_cast<Network::PlayerDeathPacket>(packet);
    if (!playerDeathPacket)
        return;

    const std::string &username = playerDeathPacket->getUsername();
    auto entity = entityManager.entities.begin();
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

void MessageSystem::handleBulletHit(EntityManager &entityManager, const std::shared_ptr<Network::Packet> &packet)
{
    auto bulletHitPacket = std::dynamic_pointer_cast<Network::BulletHitPacket>(packet);
    if (!bulletHitPacket)
        return;

    const std::string &bulletId = bulletHitPacket->getBulletId();
    auto entity = entityManager.entities.begin();
    while (entity != entityManager.entities.end()) {
        auto *bulletIdComponent = entity->get()->getComponent<BulletIdComponent>();
        if (bulletIdComponent && bulletIdComponent->id == bulletId) {
            entity = entityManager.entities.erase(entity);
        } else {
            entity++;
        }
    }
}
