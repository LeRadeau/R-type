# Message Handling

## Introduction

La gestion des messages est un module central du serveur dans un environnement multijoueur. Ce module reçoit, interprète, et applique les messages envoyés par les clients, garantissant que l'état du jeu reste synchronisé entre toutes les parties connectées.

## Fonctionnement

### Réception des messages

Les messages sont reçus via `NetworkManager` et ajoutés à une file d'attente sécurisée (`TSQueue`).

```cpp
void receiveMessages() {
    char data[1024];
    std::size_t received;
    sf::IpAddress sender;
    unsigned short senderPort;

    while (running) {
        if (socket.receive(data, sizeof(data), received, sender, senderPort) == sf::Socket::Done) {
            std::lock_guard<std::mutex> lock(queueMutex);
            messageQueue.push(std::string(data, received));
        }
    }
}
```

### Décodage et traitement

Chaque message extrait de la file est décodé et traité selon son type (`PacketType`).

```cpp
void handleMessage(const std::string &message) {
    const char *ptr = message.data();
    PacketType type = static_cast<PacketType>(Serializer::deserialize<uint8_t>(ptr));

    switch (type) {
        case PacketType::PlayerMove:
            handlePlayerMove(ptr);
            break;
        case PacketType::PlayerShoot:
            handlePlayerShoot(ptr);
            break;
        case PacketType::PlayerDisconnect:
            handlePlayerDisconnect(ptr);
            break;
        default:
            std::cerr << "Message inconnu reçu." << std::endl;
            break;
    }
}
```

### Gestion des types de messages

#### Exemple : Traitement d'un mouvement

```cpp
void handlePlayerMove(const char *&ptr) {
    std::string playerId = Serializer::deserialize<std::string>(ptr);
    float x = Serializer::deserialize<float>(ptr);
    float y = Serializer::deserialize<float>(ptr);

    auto &player = gameState.getPlayer(playerId);
    player.setPosition({x, y});
}
```

#### Exemple : Gestion d'une déconnexion

```cpp
void handlePlayerDisconnect(const char *&ptr) {
    std::string playerId = Serializer::deserialize<std::string>(ptr);
    gameState.removePlayer(playerId);
    broadcastPlayerDisconnect(playerId);
}
```

### Diffusion des mises à jour

Après traitement, les informations mises à jour sont envoyées aux clients via `Broadcast`.

```cpp
void broadcastPlayerDisconnect(const std::string &playerId) {
    PlayerDisconnectPacket packet(playerId);
    for (const auto &client : clients) {
        networkManager.sendPacket(packet, client.getIp(), client.getPort());
    }
}
```

## Interactions

- **Avec NetworkManager** : Récupère les messages réseau et envoie les réponses.
- **Avec GameState** : Met à jour les entités et synchronise les états.
- **Avec Broadcast** : Diffuse les informations pertinentes aux autres clients.

## Exemple de flux

1. Un client envoie un message `PlayerMovePacket` au serveur.
2. Le serveur traite le message, met à jour la position du joueur, et diffuse un `PlayersUpdatePacket`.
3. Les clients reçoivent la mise à jour et synchronisent leurs entités.

## Points forts

- **Modularité** : Chaque type de message est traité indépendamment.
- **Scalabilité** : Capable de gérer plusieurs types de messages et de nombreux clients simultanément.
- **Robustesse** : Inclut des vérifications pour éviter les erreurs dues à des messages malformés.

---

Le module de gestion des messages garantit une communication fluide et cohérente entre le serveur et les clients, assurant une synchronisation optimale dans un environnement multijoueur.

