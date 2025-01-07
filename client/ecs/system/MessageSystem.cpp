#include "MessageSystem.hpp"
#include "Serializer.hpp"
#include "ecs/components.hpp"
#include "network_types.hpp"

void MessageSystem::update(EntityManager &entityManager, NetworkManager &networkManager, std::string localUsername)
{
    auto receivedMessages = networkManager.getReceivedMessages();

    while (!receivedMessages.empty()) {
        auto message = receivedMessages.front();
        receivedMessages.pop();
        const char *ptr = message.data();
        auto messageType = static_cast<MessageType>(Serializer::deserialize<uint8_t>(ptr));

        switch (messageType) {
            case MessageType::UPDATE_CLIENTS: {
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
                        auto &newEntity = entityManager.createEntity();
                        newEntity.addComponent<PositionComponent>(x, y);
                        newEntity.addComponent<RenderComponent>(20, sf::Color::Blue);
                        newEntity.addComponent<usernameComponent>(username);
                    }
                }
                break;
            }
            case MessageType::UPDATE_BULLETS: {
                auto numBullets = Serializer::deserialize<uint32_t>(ptr);
                for (uint32_t i = 0; i < numBullets; ++i) {
                    auto id = Serializer::deserializeString(ptr);
                    float x = Serializer::deserialize<float>(ptr);
                    float y = Serializer::deserialize<float>(ptr);
                    float vx = Serializer::deserialize<float>(ptr);
                    float vy = Serializer::deserialize<float>(ptr);

                    Entity *bulletEntity = nullptr;

                    for (auto &entity : entityManager.entities) {
                        auto *bulletId = entity->getComponent<BulletId>();
                        if (bulletId && bulletId->id == id) {
                            bulletEntity = entity.get();
                            break;
                        }
                    }

                    if (!bulletEntity) {
                        auto &newEntity = entityManager.createEntity();
                        newEntity.addComponent<PositionComponent>(x, y);
                        newEntity.addComponent<RenderComponent>(5, sf::Color::Red);
                        newEntity.addComponent<BulletId>(id);
                        newEntity.addComponent<VelocityComponent>(vx, vy);
                    }
                }
                break;
            }
            case MessageType::ERROR:
                std::cerr << "Received error message from server" << Serializer::deserializeString(ptr) << std::endl;
                break;
            case MessageType::UPDATE_ENEMIES: {
                auto numEnnemies = Serializer::deserialize<uint32_t>(ptr);
                std::cout << "Received " << numEnnemies << " ennemies" << std::endl;
                for (uint32_t i = 0; i < numEnnemies; ++i) {
                    auto id = Serializer::deserializeString(ptr);
                    float x = Serializer::deserialize<float>(ptr);
                    float y = Serializer::deserialize<float>(ptr);
                    int health = Serializer::deserialize<int>(ptr);

                    Entity *ennemyEntity = nullptr;

                    for (auto &entity : entityManager.entities) {
                        auto *ennemyId = entity->getComponent<EnnemyId>();
                        if (ennemyId && ennemyId->id == id) {
                            ennemyEntity = entity.get();
                            break;
                        }
                    }

                    if (ennemyEntity) {
                        auto *position = ennemyEntity->getComponent<PositionComponent>();
                        auto *healthComp = ennemyEntity->getComponent<HealthComponent>();
                        if (position) {
                            position->position.x = x;
                            position->position.y = y;
                        }
                        if (healthComp) {
                            healthComp->health = health;
                        }
                    }

                    if (!ennemyEntity) {
                        auto &newEntity = entityManager.createEntity();
                        newEntity.addComponent<PositionComponent>(x, y);
                        newEntity.addComponent<RenderComponent>(20, sf::Color::Red);
                        newEntity.addComponent<EnnemyId>(id);
                        newEntity.addComponent<HealthComponent>(health);
                    }
                }
            }
            default: break;
        }
    }
}
