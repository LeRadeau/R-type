# UsernameComponent

## Introduction

Le composant `UsernameComponent` est utilisé pour associer un identifiant utilisateur unique à une entité. Ce composant est essentiel dans un environnement multijoueur pour différencier les joueurs.

## Attributs

- **username** : Une chaîne de caractères représentant l'identifiant unique d'un joueur.

### Exemple d'attributs dans le fichier `UsernameComponent.hpp` :

```cpp
class UsernameComponent : public AComponent {
public:
    std::string username;

    UsernameComponent(const std::string &username) : username(username) {}
};
```

## Rôle dans l'ECS

`UsernameComponent` est utilisé pour :

- Identifier de manière unique les joueurs dans le système.
- Faciliter les interactions entre joueurs et la gestion des scores, des statistiques ou d'autres informations spécifiques.
- Synchroniser les informations utilisateur entre les clients et le serveur dans un jeu en réseau.

## Exemples d'Utilisation

1. **Ajout à une entité** :
   ```cpp
   Entity &player = entityManager.createEntity();
   player.addComponent<UsernameComponent>("Player1");
   ```

2. **Récupération du nom d'utilisateur** :
   ```cpp
   auto *username = entity->getComponent<UsernameComponent>();
   if (username) {
       std::cout << "Nom d'utilisateur : " << username->username << std::endl;
   }
   ```

3. **Utilisation dans un système** :
   Dans un système comme `MessageSystem`, ce composant peut être utilisé pour associer les actions ou événements à un joueur spécifique :
   ```cpp
   auto *username = entity->getComponent<UsernameComponent>();
   if (username) {
       std::cout << "Synchronisation des données pour : " << username->username << std::endl;
   }
   ```

## Interactions

- **Avec NetworkComponent** : Associe le nom d'utilisateur à des données réseau spécifiques pour la synchronisation.
- **Avec RenderSystem** : Affiche le nom d'utilisateur au-dessus de l'entité correspondante.

---

Le composant `UsernameComponent` est essentiel pour identifier les joueurs dans un environnement multijoueur et gérer les interactions spécifiques à chaque utilisateur.

