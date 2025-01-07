#pragma once
#include "ecs.hpp"
#include "components.hpp"
#include "network.hpp"
#include <SFML/Graphics.hpp>
#include "./../common/Serializer.hpp"
#include "./../common/network_types.hpp"

class RenderSystem {
public:
    RenderSystem(sf::RenderWindow& window) : window(window) {
        font.loadFromFile("arial.ttf");
    }

    void displayBarInfos(EntityManager& entityManager) {
        sf::RectangleShape bar = sf::RectangleShape(sf::Vector2f(window.getSize().x, 70));
        bar.setFillColor(sf::Color::Blue);
        window.draw(bar);
        int offset = 0;
        int usernameOffset = 0;
        for (auto& entity : entityManager.entities) {
            auto* username = entity->getComponent<usernameComponent>();

            if (username) {
                sf::Text text;
                text.setFont(font);
                text.setString(username->username);
                text.setCharacterSize(24);
                text.setFillColor(sf::Color::White);
                text.setPosition(offset, 10);
                window.draw(text);
                usernameOffset = (text.getLocalBounds().width + 10);
                text.setString("HP: 100");
                text.setPosition(offset, 40);
                window.draw(text);
                offset += (text.getLocalBounds().width + 10) < usernameOffset ? usernameOffset : (text.getLocalBounds().width + 10);
            }
        }
    }

    void update(EntityManager& entityManager) {
        for (auto& entity : entityManager.entities) {
            auto* position = entity->getComponent<PositionComponent>();
            auto* render = entity->getComponent<RenderComponent>();
            auto* username = entity->getComponent<usernameComponent>();
            auto* health = entity->getComponent<HealthComponent>();
            
            if (position && render && username && health) {
                render->shape.setPosition(position->position);
                window.draw(render->shape);

                sf::Text text;
                text.setFont(font);
                text.setString(username->username);
                text.setCharacterSize(24);
                text.setFillColor(sf::Color::White);
                text.setPosition(position->position.x, position->position.y - 30);
                window.draw(text);

                text.setString("HP: " + std::to_string(health->health));
                text.setPosition(position->position.x, position->position.y + 30);
                window.draw(text);
            } else if (position && render && health) {
                render->shape.setPosition(position->position);
                window.draw(render->shape);

                sf::Text text;
                text.setFont(font);
                text.setString("HP: " + std::to_string(health->health));
                text.setPosition(position->position.x, position->position.y + 30);
                window.draw(text);
            } else if (position && render && username) {
                render->shape.setPosition(position->position);
                window.draw(render->shape);

                sf::Text text;
                text.setFont(font);
                text.setString(username->username);
                text.setCharacterSize(24);
                text.setFillColor(sf::Color::White);
                text.setPosition(position->position.x, position->position.y - 30);
                window.draw(text);
            } else if (position && render) {
                render->shape.setPosition(position->position);
                window.draw(render->shape);
            }
            displayBarInfos(entityManager);
        }
    }

private:
    sf::RenderWindow& window;
    sf::Font font;
};

class InputSystem {
public:
    void update(EntityManager& entityManager) {
        for (auto& entity : entityManager.entities) {
            auto* input = entity->getComponent<InputComponent>();
            if (input) {
                input->moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
                input->moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
                input->moveUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
                input->moveDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
                input->spaceBar = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
            }
        }
    }
};

class MovementSystem {
public:
    void update(EntityManager& entityManager, NetworkManager& networkManager, float deltaTime, bool focus) {
        static float timer = 0.0f;
        static float bulletTimer = 0.0f;

        timer += deltaTime;
        bulletTimer += deltaTime;

        for (auto& entity : entityManager.entities) {
            auto* input = entity->getComponent<InputComponent>();
            auto* position = entity->getComponent<PositionComponent>();
            auto* network = entity->getComponent<NetworkComponent>();
            auto* velocity = entity->getComponent<VelocityComponent>();
            auto* bulletId = entity->getComponent<BulletId>();

            // Movement logic for players
            if (input && position && network && (input->moveLeft || input->moveRight || input->moveUp || input->moveDown) && focus) {
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
                if (position->position.y < 0 || position->position.y > 1080 || position->position.x < 0 || position->position.x > 1920) {
                    entityManager.markForDeletion(entity->getId());
                } else {
                    position->position.x += velocity->velocity.x * deltaTime;
                    position->position.y += velocity->velocity.y * deltaTime;
                }
            }
        }
        entityManager.destroyMarkedEntities();
    }
};

class MessageSystem {
public:
    void update(EntityManager& entityManager, NetworkManager& networkManager, std::string localUsername) {
        auto receivedMessages = networkManager.getReceivedMessages();


        while (!receivedMessages.empty()) {
            auto message = receivedMessages.front();
            receivedMessages.pop();
            const char* ptr = message.data();
            auto messageType = static_cast<MessageType>(Serializer::deserialize<uint8_t>(ptr));

            switch (messageType) {
                case MessageType::UPDATE_CLIENTS: {
                    auto numClients = Serializer::deserialize<uint32_t>(ptr);
                    for (uint32_t i = 0; i < numClients; ++i) {
                        auto username = Serializer::deserializeString(ptr);
                        auto x = Serializer::deserialize<float>(ptr);
                        auto y = Serializer::deserialize<float>(ptr);
                        Entity* clientEntity = nullptr;

                        for (auto& entity : entityManager.entities) {
                            auto* usernameComp = entity->getComponent<usernameComponent>();
                            if (usernameComp && usernameComp->username == username) {
                                clientEntity = entity.get();
                                break;
                            }
                        }
                        if (clientEntity) {
                            auto* position = clientEntity->getComponent<PositionComponent>();
                            if (position && localUsername != username) {
                                position->position.x = x;
                                position->position.y = y;
                            }
                        } else {
                            auto& newEntity = entityManager.createEntity();
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

                        Entity* bulletEntity = nullptr;

                        for (auto& entity : entityManager.entities) {
                            auto* bulletId = entity->getComponent<BulletId>();
                            if (bulletId && bulletId->id == id) {
                                bulletEntity = entity.get();
                                break;
                            }
                        }

                        if (!bulletEntity ) {
                            auto& newEntity = entityManager.createEntity();
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
                case MessageType::UPDATE_ENEMIES:
                    {
                        auto numEnnemies = Serializer::deserialize<uint32_t>(ptr);
                        std::cout << "Received " << numEnnemies << " ennemies" << std::endl;
                        for (uint32_t i = 0; i < numEnnemies; ++i) {
                            auto id = Serializer::deserializeString(ptr);
                            float x = Serializer::deserialize<float>(ptr);
                            float y = Serializer::deserialize<float>(ptr);
                            int health = Serializer::deserialize<int>(ptr);

                            Entity* ennemyEntity = nullptr;

                            for (auto& entity : entityManager.entities) {
                                auto* ennemyId = entity->getComponent<EnnemyId>();
                                if (ennemyId && ennemyId->id == id) {
                                    ennemyEntity = entity.get();
                                    break;
                                }
                            }

                            if (ennemyEntity) {
                                auto* position = ennemyEntity->getComponent<PositionComponent>();
                                auto* healthComp = ennemyEntity->getComponent<HealthComponent>();
                                if (position) {
                                    position->position.x = x;
                                    position->position.y = y;
                                }
                                if (healthComp) {
                                    healthComp->health = health;
                                }
                            }

                            if (!ennemyEntity) {
                                auto& newEntity = entityManager.createEntity();
                                newEntity.addComponent<PositionComponent>(x, y);
                                newEntity.addComponent<RenderComponent>(20, sf::Color::Red);
                                newEntity.addComponent<EnnemyId>(id);
                                newEntity.addComponent<HealthComponent>(health);
                            }
                        }
                    }
                default:
                    break;
            }
        }
    }
};
