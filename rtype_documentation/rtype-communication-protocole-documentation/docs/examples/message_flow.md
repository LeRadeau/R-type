# Message Flow

## Introduction

Le flux des messages est un processus essentiel pour garantir une communication fluide et efficace entre les clients et le serveur dans un jeu multijoueur. Cette section décrit les étapes du flux de messages, depuis leur création jusqu'à leur traitement et diffusion.

## Étapes du flux

### 1. Création du message

Un message est créé lorsqu'un événement se produit côté client ou serveur (ex. : déplacement, tir, connexion).

#### Exemple : Message `PlayerMovePacket`

```cpp
PlayerMovePacket movePacket("player1", {100.0f, 200.0f}, {1.0f, 0.0f});
networkManager.sendPacket(movePacket, serverIp, serverPort);
```

### 2. Envoi au serveur

Le client envoie le message via `NetworkManager`. Les données sont sérialisées et transmises au serveur.

```cpp
socket.send(buffer.data(), buffer.size(), serverIp, serverPort);
```

### 3. Réception par le serveur

Le serveur écoute les messages entrants et les place dans une file d'attente sécurisée (`TSQueue`).

```cpp
void receiveMessages() {
    char data[1024];
    std::size_t received;
    sf::IpAddress sender;
    unsigned short senderPort;

    if (socket.receive(data, sizeof(data), received, sender, senderPort) == sf::Socket::Done) {
        messageQueue.push(std::string(data, received));
    }
}
```

### 4. Décodage et traitement côté serveur

Le serveur extrait les messages de la file, identifie leur type, et applique les actions correspondantes.

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
        default:
            std::cerr << "Message inconnu." << std::endl;
    }
}
```

### 5. Mise à jour de l'état

Le serveur met à jour l'état du jeu en fonction des données du message reçu.

```cpp
void handlePlayerMove(const char *&ptr) {
    std::string playerId = Serializer::deserialize<std::string>(ptr);
    float x = Serializer::deserialize<float>(ptr);
    float y = Serializer::deserialize<float>(ptr);
    gameState.updatePlayerPosition(playerId, {x, y});
}
```

### 6. Diffusion aux clients

Le serveur crée un paquet de mise à jour et le diffuse à tous les clients connectés.

```cpp
void broadcastUpdates() {
    for (const auto &client : clients) {
        networkManager.sendPacket(updatePacket, client.getIp(), client.getPort());
    }
}
```

### 7. Réception et synchronisation côté client

Les clients reçoivent les messages diffusés par le serveur et synchronisent leur état local.

```cpp
if (packet->getType() == PacketType::PlayersUpdate) {
    auto updatePacket = dynamic_cast<PlayersUpdatePacket *>(packet.get());
    for (const auto &player : updatePacket->getPlayerData()) {
        gameState.updatePlayer(player.playerId, player.position, player.velocity);
    }
}
```

## Visualisation

```plaintext
Client 1 -----> [PlayerMovePacket] -----> Serveur -----> [PlayersUpdatePacket] -----> Tous les clients
```

## Points forts

- **Clarté** : Un flux structuré garantit une communication efficace.
- **Synchronisation** : Assure que tous les participants ont une vue cohérente de l'état du jeu.
- **Extensibilité** : Le flux est adaptable pour inclure de nouveaux types de messages.

---

Le flux des messages est un mécanisme clé pour maintenir une expérience multijoueur fluide et immersive.

