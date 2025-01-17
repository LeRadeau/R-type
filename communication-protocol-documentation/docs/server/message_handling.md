# Message Handling

## Introduction

Le module de gestion des messages est responsable du traitement des données échangées entre le serveur et les clients. Il décode les messages reçus, applique les actions correspondantes et prépare les réponses ou mises à jour à envoyer.

## Rôle dans le serveur

1. **Décodage** : Identifier le type de message et extraire les données associées.
2. **Application** : Exécuter la logique correspondante (ex. : mise à jour de position, enregistrement de tirs).
3. **Réponse** : Préparer et envoyer des messages de confirmation ou de synchronisation.

## Fonctionnalités principales

### Décodage des messages

Le traitement des messages commence par l'identification du type et la désérialisation des données.

```cpp
void MessageHandler::handleMessage(const std::string &message, sf::IpAddress senderIp, unsigned short senderPort) {
    const char *ptr = message.data();
    auto messageType = static_cast<MessageType>(Serializer::deserialize<uint8_t>(ptr));

    switch (messageType) {
        case MessageType::MOVE:
            handleMove(ptr, senderIp, senderPort);
            break;
        case MessageType::SHOOT:
            handleShoot(ptr, senderIp, senderPort);
            break;
        case MessageType::CONNECT:
            handleConnect(ptr, senderIp, senderPort);
            break;
        default:
            std::cerr << "Message inconnu reçu." << std::endl;
            break;
    }
}
```

### Gestion des types spécifiques

#### Exemple : Gestion d'un mouvement

```cpp
void MessageHandler::handleMove(const char *&ptr, sf::IpAddress senderIp, unsigned short senderPort) {
    auto playerId = Serializer::deserialize<std::string>(ptr);
    float positionX = Serializer::deserialize<float>(ptr);
    float positionY = Serializer::deserialize<float>(ptr);

    auto &player = gameState.getPlayer(playerId);
    player.position.x = positionX;
    player.position.y = positionY;

    std::cout << "Joueur " << playerId << " déplacé en (" << positionX << ", " << positionY << ")" << std::endl;
}
```

### Réponse aux clients

Après le traitement, le serveur envoie une réponse ou une mise à jour à tous les clients concernés :

```cpp
void MessageHandler::sendUpdate(const GameState &gameState) {
    for (const auto &client : gameState.clients) {
        std::string buffer;
        Serializer::serialize(buffer, static_cast<uint8_t>(MessageType::UPDATE_CLIENTS));
        Serializer::serialize(buffer, client.id);
        Serializer::serialize(buffer, client.position.x);
        Serializer::serialize(buffer, client.position.y);

        socket.send(buffer.data(), buffer.size(), client.ip, client.port);
    }
}
```

## Interactions

- **Avec GameState** : Met à jour les entités en fonction des données reçues.
- **Avec Serializer** : Encode et décode les messages réseau.
- **Avec Broadcasting** : Envoie les mises à jour aux clients après traitement.

## Exemple de flux complet

1. Un client envoie un message `MOVE` au serveur.
2. Le message est reçu et décodé dans `handleMessage`.
3. La position du joueur est mise à jour dans l'état du jeu.
4. Une mise à jour est diffusée à tous les clients.

---

La gestion des messages garantit une communication efficace et fiable entre les clients et le serveur, tout en assurant une synchronisation cohérente des états du jeu.

