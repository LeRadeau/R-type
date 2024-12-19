#pragma once
#include <string>
#include <cstring>
#include <cstdint>
#include "./NetworkManager.hpp"

class ClientNetworkSystem {
    public:
        ClientNetworkSystem(std::string serverIp, uint16_t serverPort) : serverIp(serverIp), serverPort(serverPort) {}
        bool createSocket() {
            networkManager = NetworkManager();
            return true;
        }
        void dataFromServer(EntityManager& em) {
            std::vector<Packet> packets = networkManager.receiveMessages(false);
            for (Packet packet : packets) {
                Serializer::MessageType messageType = static_cast<Serializer::MessageType>(Serializer::deserialize<uint8_t>(packet.data));
                if (messageType == Serializer::MessageType::CONNECT) {
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
                    Entity& entity = em.findEntity(entityNbr);

                    Serializer::MessageType componentType = static_cast<Serializer::MessageType>(Serializer::deserialize<Serializer::MessageType>(packet.data));
                    
                    // Component WINDOW - CREATION DE WINDOW
                    if (componentType == Serializer::MessageType::WINDOW) {
                        std::cout << "WINDOW\n";
                        unsigned int modeWidth = static_cast<unsigned int>(Serializer::deserialize<unsigned int>(packet.data));
                        unsigned int modeHeight = static_cast<unsigned int>(Serializer::deserialize<unsigned int>(packet.data));
                        std::cout << "modeWidth -> " << modeWidth << " modeHeight -> " << modeHeight << std::endl;
                        entity.addComponent<WindowComponent>(modeWidth, modeHeight);
                    }

                    // Component WINDOW - CREATION DE WINDOW
                    if (componentType == Serializer::MessageType::RENDER) {
                        std::cout << "RENDER\n";
                        std::string pathImg = Serializer::deserializeString(packet.data);
                        std::cout << "Pathimg -> " << pathImg << std::endl;
                        entity.addComponent<RenderComponent>(pathImg, true);
                    }
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
};