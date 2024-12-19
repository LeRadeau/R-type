#pragma once
#include <string>
#include <cstring>
#include <cstdint>
#include "./NetworkManager.hpp"
#include "../../components/Input/InputComponent.hpp"
#include "../../components/Transform/PositionComponent.hpp"

class ClientNetworkSystem {
    public:
        ClientNetworkSystem(std::string serverIp, uint16_t serverPort, float coolDown = 0) : serverIp(serverIp), serverPort(serverPort), coolDown(coolDown) {}
        bool createSocket() {
            networkManager = NetworkManager();
            return true;
        }

        void dataToServer(EntityManager& em, InputSystem& inputSystem, float dt) {
            if (coolDown != 0) {
                currentTime += dt;
                if (currentTime >= coolDown) {
                    // std::cout << currentTime << std::endl;
                    currentTime = 0;
                } else {
                    return;
                }
            }

            if (inputSystem.inputPress) {
                inputSystem.inputPress = false;
                for (auto& entity : em.entities) {
                    std::string buffer;
                    auto* input = entity->getComponent<InputComponent>();
                    auto* position = entity->getComponent<PositionComponent>();

                    if (input && position) {
                        Serializer::serialize(buffer, Serializer::MessageType::ENTITY);
                        Serializer::serialize(buffer, (uint64_t)entity->getId());
                        Serializer::serialize(buffer, Serializer::MessageType::POSITION);
                        Serializer::serialize(buffer, (float) position->position.x);
                        Serializer::serialize(buffer, (float) position->position.y);
                        Serializer::serialize(buffer, Serializer::MessageType::END);
                        networkManager.sendTo(buffer, serverIp, serverPort);
                    }

                }
            }
        }

        // Process of deserialization and Creation/Modification of entity/component
        void dataFromServer(EntityManager& em) {
            std::vector<Packet> packets = networkManager.receiveMessages(false);
            for (Packet packet : packets) {
                Serializer::MessageType messageType = Serializer::MessageType::NOTHING;
                while (1) {
                    messageType = static_cast<Serializer::MessageType>(Serializer::deserialize<uint8_t>(packet.data));
                    if (messageType == Serializer::MessageType::END)
                        break;
                    if (messageType == Serializer::MessageType::CONNECTED) {
                        // ECS connection good
                        std::cout << "Connected to server successful" << std::endl;
                    }
                    // Creation of modification of entity
                    if (messageType == Serializer::MessageType::ENTITY) {
                        uint64_t entityNbr = static_cast<uint64_t>(Serializer::deserialize<uint64_t>(packet.data));
                        if (em.checkIfEntityExist(entityNbr)) {
                            // Créer l'entité avec son ID
                            std::cout << "entité trouvé\n";
                        } else {
                            // On est sensé le crée
                            std::cout << "entité pas trouvé, entité crée du coup\n";
                            continue;
                        }
                        std::unique_ptr<Entity>& entity = em.findEntity(entityNbr);
                        while (1) {
                            messageType = static_cast<Serializer::MessageType>(Serializer::deserialize<Serializer::MessageType>(packet.data));
                            if (messageType == Serializer::MessageType::END || messageType == Serializer::MessageType::NEXT)
                                break;
                            // Component WINDOW
                            if (messageType == Serializer::MessageType::WINDOW) {
                                std::cout << "WINDOW\n";
                                unsigned int modeWidth = static_cast<unsigned int>(Serializer::deserialize<unsigned int>(packet.data));
                                unsigned int modeHeight = static_cast<unsigned int>(Serializer::deserialize<unsigned int>(packet.data));
                                std::cout << "modeWidth -> " << modeWidth << " modeHeight -> " << modeHeight << std::endl;
                                auto* win = entity->getComponent<WindowComponent>();
                                if (!win) {
                                    entity->addComponent<WindowComponent>(modeWidth, modeHeight);
                                }  
                            }

                            // Component RENDER
                            if (messageType == Serializer::MessageType::RENDER) {
                                std::cout << "RENDER\n";
                                std::string pathImg = Serializer::deserializeString(packet.data);
                                std::cout << "Pathimg -> " << pathImg << std::endl;
                                auto* render = entity->getComponent<RenderComponent>();
                                if (!render) {
                                    entity->addComponent<RenderComponent>(pathImg, true);
                                } else {
                                    std::cout << "Il faut modifier le render\n";
                                }
                            }

                            // Component INPUT
                            if (messageType == Serializer::MessageType::INPUT) {
                                std::cout << "INPUT\n";
                                auto* input = entity->getComponent<InputComponent>();
                                if (!input) {
                                entity->addComponent<InputComponent>();
                                } else {
                                    std::cout << "Il faut modifier le INPUT\n";
                                }
                            }
                            //Component POS
                            if (messageType == Serializer::MessageType::POSITION) {
                                std::cout << "POSITION\n";
                                sf::Vector2f pos;
                                float x = static_cast<float>(Serializer::deserialize<float>(packet.data));
                                float y = static_cast<float>(Serializer::deserialize<float>(packet.data));
                                std::cout << "x -> " << x << " y -> " << y << std::endl;
                                auto* input = entity->getComponent<PositionComponent>();
                                if (!input) {
                                entity->addComponent<PositionComponent>(x, y);
                                } else {
                                    std::cout << "Il faut modifier le INPUT\n";
                                }
                            }
                        }
                    }
                    if (messageType == Serializer::MessageType::END)
                        break;
                }
                free((void *)packet.ptr);
            }
        }
        void test() {
            std::string buffer;
            Serializer::serialize(buffer, Serializer::MessageType::CONNECT);
            Serializer::serialize(buffer, Serializer::MessageType::END);
            networkManager.sendTo(buffer, serverIp, serverPort);
        }
    private:
        std::string serverIp;
        uint16_t serverPort;
        NetworkManager networkManager;

        float coolDown; // Time passed
        float currentTime; // Current time
};