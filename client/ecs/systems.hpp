#pragma once
#include "ecs.hpp"
#include "components.hpp"
#include "network.hpp"
#include <SFML/Graphics.hpp>
#include "./../common/Serializer.hpp"

class RenderSystem {
public:
    RenderSystem(sf::RenderWindow& window) : window(window) {}

    void update(EntityManager& entityManager) {
        for (auto& entity : entityManager.entities) {
            auto* position = entity->getComponent<PositionComponent>();
            auto* render = entity->getComponent<RenderComponent>();
            auto* username = entity->getComponent<usernameComponent>();
            
            if (position && render) {
                render->shape.setPosition(position->position);
                window.draw(render->shape);
            }
        }
    }

private:
    sf::RenderWindow& window;
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
    void update(EntityManager& entityManager, NetworkManager& networkManager, float deltaTime) {
        static float timer = 0.0f;

        timer += deltaTime;
        for (auto& entity : entityManager.entities) {
            auto* input = entity->getComponent<InputComponent>();
            auto* position = entity->getComponent<PositionComponent>();
            auto* network = entity->getComponent<NetworkComponent>();
            
            if (input && position && network && (input->moveLeft || input->moveRight || input->moveUp || input->moveDown)) {
                if (input->moveLeft) 
                    position->position.x -= 200.0f * deltaTime;
                if (input->moveRight)
                    position->position.x += 200.0f * deltaTime;
                if (input->moveUp)
                    position->position.y -= 200.0f * deltaTime;
                if (input->moveDown)
                    position->position.y += 200.0f * deltaTime;
                
                if (timer > 0.1f) {
                    std::string buffer;
                    Serializer::serialize(buffer, static_cast<uint8_t>(Serializer::MessageType::MOVE));
                    Serializer::serialize(buffer, network->username);
                    Serializer::serialize(buffer, position->position.x);
                    Serializer::serialize(buffer, position->position.y);
                    networkManager.send(buffer);
                    timer = 0.0f;
                }
            }
        }
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
            auto messageType = static_cast<Serializer::MessageType>(Serializer::deserialize<uint8_t>(ptr));

            switch (messageType) {
                case Serializer::MessageType::UPDATE_CLIENTS: {
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

                        std::cout << "Updated position for " << username << ": " << x << ", " << y << std::endl;
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }
};
