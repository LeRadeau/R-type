# NetworkCallbackComponent

## Introduction

Le composant `NetworkCallbackComponent` permet d'associer des callbacks réseau à une entité. Ces callbacks sont exécutés lorsqu'un message spécifique est reçu du serveur, facilitant la gestion des interactions réseau pour une entité donnée.

## Attributs

- **callbacks** : Une table associative reliant un type de message (`MessageType`) à une fonction de callback.

### Exemple d'attributs dans le fichier `NetworkCallbackComponent.hpp` :

```cpp
#include <unordered_map>
#include <functional>
#include "network_types.hpp"

class NetworkCallbackComponent : public AComponent {
public:
    using callbackFunction = std::function<void(const char *&)>;

    std::unordered_map<MessageType, callbackFunction> callbacks;

    void setCallback(MessageType messageType, callbackFunction callback) {
        callbacks[messageType] = callback;
    }
};
```

## Rôle dans l'ECS

`NetworkCallbackComponent` est utilisé pour :

- Gérer des actions spécifiques en réponse à des messages réseau.
- Associer des comportements dynamiques aux entités basés sur des messages du serveur.
- Centraliser la logique réseau des entités.

## Exemples d'Utilisation

1. **Ajout d'un composant réseau** :
   ```cpp
   auto &networkCallback = entity.addComponent<NetworkCallbackComponent>();
   networkCallback.setCallback(MessageType::UPDATE_CLIENTS, [](const char *&packet) {
       std::cout << "Client list updated!" << std::endl;
   });
   ```

2. **Exécution d'un callback** :
   Lorsqu'un message est reçu, la fonction associée est exécutée :
   ```cpp
   if (callbacks.find(messageType) != callbacks.end()) {
       callbacks[messageType](packet);
   }
   ```

## Interactions

- **Avec MessageSystem** : Parcourt les entités et exécute les callbacks réseau associés en fonction des messages reçus.
- **Avec NetworkManager** : Reçoit les données du serveur pour déclencher les callbacks pertinents.

## Fonctionnalités supplémentaires

- **Extensibilité** : Permet d'ajouter facilement de nouveaux comportements liés à des types de messages spécifiques.
- **Centralisation de la logique réseau** : Simplifie la gestion des interactions serveur-client au niveau des entités.

---

`NetworkCallbackComponent` est un composant puissant pour gérer la logique réseau au sein du jeu, offrant flexibilité et modularité pour répondre aux événements du serveur.

