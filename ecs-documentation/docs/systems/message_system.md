# MessageSystem

## Introduction

Le système `MessageSystem` est conçu pour gérer la communication entre le client et le serveur. Il permet de recevoir et de traiter les messages réseau, tout en mettant à jour les entités du jeu en conséquence.

## Rôle dans l'ECS

`MessageSystem` est responsable de :

- Récupérer les messages reçus via le réseau.
- Décoder et interpréter les messages pour mettre à jour les entités concernées.
- Envoyer les réponses ou les mises à jour nécessaires au serveur.

## Fonctionnalités principales

### Méthode `update`

La méthode `update` traite les messages en file d'attente, identifie leur type, et invoque les fonctions appropriées pour mettre à jour les entités.

#### Exemple de code

```cpp
void MessageSystem::update(EntityManager &entityManager, NetworkManager &networkManager, std::string localUsername) {
    auto receivedMessages = networkManager.getReceivedMessages();

    while (!receivedMessages.empty()) {
        auto message = receivedMessages.front();
        receivedMessages.pop();
        const char *ptr = message.data();
        auto messageType = static_cast<MessageType>(Serializer::deserialize<uint8_t>(ptr));

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
            case MessageType::UPDATE_ENEMIES:
                handleUpdateEnemies(entityManager, ptr);
                break;
            default:
                break;
        }
    }
}
```

### Méthodes de traitement

- **`handleUpdateClients`** : Met à jour les positions et les données des clients.
- **`handleUpdateBullets`** : Ajoute ou met à jour les projectiles dans le jeu.
- **`handleError`** : Affiche les messages d'erreur reçus.
- **`handleUpdateEnemies`** : Synchronise les données des ennemis avec celles reçues du serveur.

## Exemples d'Utilisation

1. **Initialisation et appel** :
   ```cpp
   MessageSystem messageSystem;
   messageSystem.update(entityManager, networkManager, "Player1");
   ```

2. **Traitement des erreurs** :
   ```cpp
   void MessageSystem::handleError(const char *&ptr) {
       std::cerr << "Erreur reçue : " << Serializer::deserializeString(ptr) << std::endl;
   }
   ```

## Interactions

- **Avec NetworkManager** : Récupère les messages du réseau et envoie les réponses.
- **Avec EntityManager** : Met à jour ou crée des entités en fonction des messages reçus.
- **Avec Serializer** : Utilisé pour coder et décoder les données des messages.

---

Le système `MessageSystem` joue un rôle central dans la gestion des communications réseau, garantissant une synchronisation fluide entre le client et le serveur.

