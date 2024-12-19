#pragma once
#include <string>
#include <cstring>
#include <cstdint>
#include "./NetworkManager.hpp"
#include "../../components/Input/InputComponent.hpp"
#include "../../components/Transform/PositionComponent.hpp"
#include "../../components/Network/BindRemoteComponent.hpp"
#include "../../components/Render/RenderComponent.hpp"
#include "../../components/Render/WindowComponent.hpp"

class ServerNetworkSystem {
    public:
        ServerNetworkSystem(std::string serverIp, uint16_t serverPort, float coolDown = 0) : serverIp(serverIp), serverPort(serverPort), coolDown(coolDown) {
            networkManager.bindSocket(serverIp, serverPort);
        }
        bool createSocket() {
            networkManager = NetworkManager();
            return true;
        }

        void dataToClients(EntityManager& em, float dt) {
            if (coolDown != 0) {
                currentTime += dt;
                if (currentTime >= coolDown) {
                    // std::cout << currentTime << std::endl;
                    currentTime = 0;
                } else {
                    return;
                }
            }
            
            int next = false;
            for (auto& entity : em.entities) {
                auto *currentbindClient = entity->getComponent<BindClientComponent>();
                if (currentbindClient && currentbindClient->connected) {
                    for (auto& entityy : em.entities) {
                        std::string buffer;
                        auto* window = entityy->getComponent<WindowComponent>();
                        auto* input = entityy->getComponent<InputComponent>();
                        auto* render = entityy->getComponent<RenderComponent>();
                        auto* position = entityy->getComponent<PositionComponent>();
                        auto *bindClient = entityy->getComponent<BindClientComponent>();

                        Serializer::serialize(buffer, Serializer::MessageType::ENTITY);
                        Serializer::serialize(buffer, (uint64_t)entityy->getId());
                        if (window) {
                            std::cout << " WINDOW " << entityy->getId() << " |";
                            Serializer::serialize(buffer, Serializer::MessageType::WINDOW);
                            Serializer::serialize(buffer, (unsigned int) window->modeWidth);
                            Serializer::serialize(buffer, (unsigned int) window->modeHeight);
                        }
                        if (input && currentbindClient == bindClient) {
                            std::cout << " INPUT " << entityy->getId() << " |";
                            Serializer::serialize(buffer, Serializer::MessageType::INPUT);
                        }
                        if (render) {
                            std::cout << " RENDER " << entityy->getId() << " |";
                            Serializer::serialize(buffer, Serializer::MessageType::RENDER);
                            Serializer::serialize(buffer, (std::string) render->pathTexture);
                        }
                        if (position && currentbindClient == bindClient) {
                            std::cout << " POS " << entityy->getId() << " |";
                            Serializer::serialize(buffer, Serializer::MessageType::POSITION);
                            Serializer::serialize(buffer, (float) position->position.x);
                            Serializer::serialize(buffer, (float) position->position.y); 
                        }
                        std::cout << std::endl;
                        Serializer::serialize(buffer, Serializer::MessageType::END);
                        networkManager.sendTo(buffer, currentbindClient->ipClient, currentbindClient->portClient);
                    }
                }
            }
        }

        void dataFromClients(EntityManager& em) {
            std::vector<Packet> packets = networkManager.receiveMessages(true);
            for (Packet packet : packets) {
                Serializer::MessageType messageType = Serializer::MessageType::NOTHING;
                while (1) {
                    messageType = static_cast<Serializer::MessageType>(Serializer::deserialize<uint8_t>(packet.data));
                    if (messageType == Serializer::MessageType::END)
                        break;
                    if (messageType == Serializer::MessageType::CONNECT) {
                        for (auto& entity : em.entities) {
                            auto *bindClient = entity->getComponent<BindClientComponent>();
                            if (bindClient && !bindClient->connected) {
                                bindClient->ipClient = packet.senderIp;
                                bindClient->portClient = packet.senderPort;
                                bindClient->connected = true;
                                std::cout << "Connected to BIND Client to Server OK " << std::endl;
                                std::string buffer;
                                Serializer::serialize(buffer, Serializer::MessageType::CONNECTED);
                                Serializer::serialize(buffer, Serializer::MessageType::END);
                                networkManager.sendTo(buffer, bindClient->ipClient, bindClient->portClient);
                                break;
                            }
                        }
                        std::cout << "Impossible to bind client\n";
                        return;
                    }
                    if (messageType == Serializer::MessageType::ENTITY) {
                        uint64_t entityNbr = static_cast<uint64_t>(Serializer::deserialize<uint64_t>(packet.data));
                        std::unique_ptr<Entity>& entity = em.findEntity(entityNbr);
                        auto *bindClient = entity->getComponent<BindClientComponent>();
                        messageType = static_cast<Serializer::MessageType>(Serializer::deserialize<uint8_t>(packet.data));
                        if (messageType == Serializer::MessageType::POSITION)
                            if (bindClient->ipClient == packet.senderIp && bindClient->portClient == packet.senderPort) {
                                float x = static_cast<float>(Serializer::deserialize<float>(packet.data));
                                float y = static_cast<float>(Serializer::deserialize<float>(packet.data));
                                auto* pos = entity->getComponent<PositionComponent>();
                                std::cout << "x -> " << x << " y -> " << y << std::endl;
                                if (pos) {
                                    pos->position.x = x;
                                    pos->position.y = y;
                                }
                            }
                    }

                    if (messageType == Serializer::MessageType::END)
                        break;
                }
            }
        }
        std::string serverIp;
        uint16_t serverPort;
        NetworkManager networkManager;

        float coolDown; // Time passed
        float currentTime; // Current time
};