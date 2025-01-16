# NetworkComponent

## Introduction

Le composant `NetworkComponent` gère les informations réseau associées à une entité, telles que l'identifiant utilisateur ou d'autres données nécessaires pour la synchronisation entre le client et le serveur.

## Attributs

- **username** : Une chaîne de caractères représentant l'identifiant unique de l'utilisateur.

### Exemple d'attributs dans le fichier `NetworkComponent.hpp` :

```cpp
class NetworkComponent : public AComponent {
public:
    std::string username;

    NetworkComponent(const std::string &username) : username(username) {}
};
```

## Rôle dans l'ECS

`NetworkComponent` est utilisé pour :

- Associer un identifiant réseau unique à une entité.
- Faciliter la synchronisation des entités entre les différents clients et le serveur.
- Identifier les actions et événements liés à un utilisateur particulier.

## Exemples d'Utilisation

1. **Ajout à une entité** :
   ```cpp
   Entity &player = entityManager.createEntity();
   player.addComponent<NetworkComponent>("player_1");
   ```

2. **Utilisation dans un système** :
   Dans un système comme `MessageSystem`, le composant peut être utilisé pour identifier et mettre à jour les entités correspondantes :
   ```cpp
   auto *network = entity->getComponent<NetworkComponent>();
   if (network) {
       std::cout << "Synchronisation pour l'utilisateur : " << network->username << std::endl;
   }
   ```

## Interactions

- **Avec MessageSystem** : Utilisé pour synchroniser l'état des entités à travers le réseau.
- **Avec InputComponent** : Relie les commandes d'un utilisateur aux entités spécifiques.
- **Avec PositionComponent** : Gère la mise à jour de la position d'une entité sur le réseau.

---

Le composant `NetworkComponent` est essentiel pour gérer la communication et la synchronisation réseau, garantissant que chaque utilisateur interagit correctement avec le jeu dans un environnement multijoueur.

