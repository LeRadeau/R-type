# MessageSystem

## Introduction

Le système `MessageSystem` côté client est chargé de gérer les messages reçus du serveur et de coordonner les actions en conséquence. Il joue un rôle clé dans la synchronisation de l'état du jeu entre le client et le serveur.

## Rôle dans l'ECS

- **Réception** : Interprète les messages envoyés par le serveur.
- **Action** : Met à jour les entités et composants du client.
- **Communication** : Envoie des réponses ou des mises à jour au serveur.

## Fonctionnalités principales

### Traitement des messages reçus

`MessageSystem` lit les messages de la file d'attente et applique les actions nécessaires via des composants réseau et des callbacks.

```cpp
void MessageSystem::update(EntityManager &entityManager, NetworkManager &networkManager, std::string localUsername,
    std::unique_ptr<PlayerEntity> &player) {
    auto &receivedMessages = networkManager.getReceivedMessages();

    while (!receivedMessages.empty()) {
        auto message = receivedMessages.pop();
        const char *ptr = message.data();
        auto messageType = static_cast<MessageType>(Serializer::deserialize<uint8_t>(ptr));

        for (auto &entity : entityManager.entities) {
            auto *networkCallbackComponent = entity->getComponent<NetworkCallbackComponent>();
            if (networkCallbackComponent && networkCallbackComponent->callbacks.count(messageType)) {
                networkCallbackComponent->callbacks.at(messageType)(ptr);
            }
        }

        switch (messageType) {
            case MessageType::UPDATE_CLIENTS:
                handleUpdateClients(entityManager, ptr, localUsername);
                break;
            case MessageType::UPDATE_BULLETS:
                handleUpdateBullets(entityManager, ptr);
                break;
            case MessageType::ERROR:
                handleError(ptr);
                break;
            default:
                break;
        }
    }
}
```

### Envoi de messages

Le système utilise `NetworkManager` pour envoyer des messages au serveur.

```cpp
void MessageSystem::sendMessage(NetworkManager &networkManager, const std::string &message) {
    networkManager.send(message);
}
```

## Interactions

- **Avec EntityManager** : Met à jour les entités en fonction des messages reçus.
- **Avec NetworkManager** : Gère la réception et l'envoi des messages réseau.
- **Avec Serializer** : Décode les messages reçus et prépare les données à envoyer.

## Exemple de flux

1. Le serveur envoie un message `UPDATE_CLIENTS` au client.
2. `MessageSystem` le reçoit et met à jour les entités via `handleUpdateClients`.
3. Les nouvelles positions des entités sont affichées à l'écran.

### Exemple d'implémentation

#### Réception d'un message `UPDATE_BULLETS`

```cpp
void MessageSystem::handleUpdateBullets(EntityManager &entityManager, const char *&ptr) {
    size_t bulletCount = Serializer::deserialize<size_t>(ptr);
    for (size_t i = 0; i < bulletCount; ++i) {
        auto bulletId = Serializer::deserialize<std::string>(ptr);
        float x = Serializer::deserialize<float>(ptr);
        float y = Serializer::deserialize<float>(ptr);
        auto *entity = entityManager.getEntityById(bulletId);
        if (entity) {
            auto *position = entity->getComponent<PositionComponent>();
            if (position) {
                position->position.x = x;
                position->position.y = y;
            }
        }
    }
}
```

---

`MessageSystem` est un système essentiel pour la gestion des communications réseau côté client, garantissant une synchronisation fluide et efficace avec le serveur.

