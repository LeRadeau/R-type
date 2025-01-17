#include "MovementSystem.hpp"
#include "Serializer.hpp"
#include "ecs/component/BulletIdComponent.hpp"
#include "ecs/component/InputComponent.hpp"
#include "ecs/component/PositionComponent.hpp"
#include "ecs/component/SoundComponent.hpp"
#include "ecs/component/UsernameComponent.hpp"
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
        auto *username = entity->getComponent<UsernameComponent>();
        auto *velocity = entity->getComponent<VelocityComponent>();
        auto *bulletId = entity->getComponent<BulletIdComponent>();
        auto *sound = entity->getComponent<SoundComponent>();

        // Movement logic for players
        bool isMoving = input && (input->moveLeft || input->moveRight || input->moveUp || input->moveDown);
        if (input && position && username && isMoving && focus) {
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
                Serializer::serialize(buffer, username->username);
                Serializer::serialize(buffer, position->position.x);
                Serializer::serialize(buffer, position->position.y);
                networkManager.send(buffer);
                timer = 0.0f;
            }
            sound->sound.setPitch(1);
        }
        if (!isMoving && sound && username) {
            sound->sound.setPitch(0.5);
        }

        // Shooting logic
        if (input && position && input->spaceBar && bulletTimer > 0.3f && focus) {
            std::string buffer;
            Serializer::serialize(buffer, static_cast<uint8_t>(MessageType::SHOOT));
            Serializer::serialize(buffer, username->username);
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
