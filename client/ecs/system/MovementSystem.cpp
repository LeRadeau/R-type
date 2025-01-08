#include "MovementSystem.hpp"
#include "Serializer.hpp"
#include "ecs/component/BulletIdComponent.hpp"
#include "ecs/component/InputComponent.hpp"
#include "ecs/component/NetworkComponent.hpp"
#include "ecs/component/PositionComponent.hpp"
#include "ecs/component/VelocityComponent.hpp"
#include "network_types.hpp"

void MovementSystem::update(EntityManager &entityManager, NetworkManager &networkManager, float deltaTime, bool focus)
{
    static float timer = 0.0f;
    static float bulletTimer = 0.0f;

    timer += deltaTime;
    bulletTimer += deltaTime;

    for (auto &entity : entityManager.entities) {
        auto *input = entity->getComponent<InputComponent>();
        auto *position = entity->getComponent<PositionComponent>();
        auto *network = entity->getComponent<NetworkComponent>();
        auto *velocity = entity->getComponent<VelocityComponent>();
        auto *bulletId = entity->getComponent<BulletIdComponent>();

        // Movement logic for players
        if (input && position && network && (input->moveLeft || input->moveRight || input->moveUp || input->moveDown)
            && focus) {
            if (input->moveLeft)
                position->position.x -= 200.0f * deltaTime;
            if (input->moveRight)
                position->position.x += 200.0f * deltaTime;
            if (input->moveUp)
                position->position.y -= 200.0f * deltaTime;
            if (input->moveDown)
                position->position.y += 200.0f * deltaTime;

            // Send position update to server
            if (timer > 0.1f) {
                std::string buffer;
                Serializer::serialize(buffer, static_cast<uint8_t>(MessageType::MOVE));
                Serializer::serialize(buffer, network->username);
                Serializer::serialize(buffer, position->position.x);
                Serializer::serialize(buffer, position->position.y);
                networkManager.send(buffer);
                timer = 0.0f;
            }
        }

        // Shooting logic
        if (input && position && input->spaceBar && bulletTimer > 0.3f && focus) {
            std::string buffer;
            Serializer::serialize(buffer, static_cast<uint8_t>(MessageType::SHOOT));
            Serializer::serialize(buffer, network->username);
            Serializer::serialize(buffer, position->position.x);
            Serializer::serialize(buffer, position->position.y);
            networkManager.send(buffer);
            bulletTimer = 0.0f;
        }

        // Movement logic for bullets
        if (position && velocity && bulletId) {
            if (position->position.y < 0 || position->position.y > 1080 || position->position.x < 0
                || position->position.x > 1920) {
                entityManager.markForDeletion(entity->getId());
            } else {
                position->position.x += velocity->velocity.x * deltaTime;
                position->position.y += velocity->velocity.y * deltaTime;
            }
        }
    }
    entityManager.destroyMarkedEntities();
}
