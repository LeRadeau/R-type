# MessageSystem

## Introduction

Le système `MessageSystem` gère les messages reçus du serveur et déclenche les actions correspondantes sur les entités. Il permet de synchroniser les états du jeu entre les clients et le serveur.

## Rôle dans l'ECS

`MessageSystem` est utilisé pour :

- Traiter les messages réseau entrants.
- Mettre à jour les composants et entités en fonction des messages reçus.
- Gérer la logique réseau et les interactions multijoueurs.

## Fonctionnalités principales

### Méthode `update`

La méthode `update` lit les messages reçus, identifie leur type, et applique les actions appropriées via des callbacks ou des mises à jour directes.

#### Code extrait de `MessageSystem.cpp`

```cpp
void MessageSystem::update(EntityManager &entityManager, NetworkManager &networkManager, std::string localUsername,
    std::unique_ptr<PlayerEntity> &player)
{
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
            case MessageType::START_GAME:
                handleLaunchGame(entityManager, player, localUsername);
                break;
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

## Interactions

- **Avec NetworkManager** : Récupère les messages réseau.
- **Avec EntityManager** : Modifie ou crée des entités en fonction des messages reçus.
- **Avec NetworkCallbackComponent** : Exécute des actions spécifiques liées aux messages.

## Exemples d'Utilisation

1. **Initialisation et mise à jour** :
   ```cpp
   MessageSystem messageSystem(font);
   messageSystem.update(entityManager, networkManager, localUsername, playerEntity);
   ```

2. **Traitement des messages réseau** :
   ```cpp
   if (messageType == MessageType::UPDATE_CLIENTS) {
       handleUpdateClients(entityManager, ptr, localUsername);
   }
   ```

## Fonctionnalités supplémentaires

- **Gestion des erreurs réseau** : Traite les messages de type `ERROR` pour afficher des messages ou effectuer des actions correctives.
- **Support multijoueur** : Synchronise les positions, scores et états des entités entre les clients.

---

`MessageSystem` est un système central pour gérer les interactions réseau dans un environnement multijoueur, assurant une synchronisation fluide et une expérience utilisateur cohérente.

