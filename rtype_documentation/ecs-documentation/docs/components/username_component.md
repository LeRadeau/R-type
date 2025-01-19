# UsernameComponent

## Introduction

Le composant `UsernameComponent` est utilisé pour associer un identifiant unique ou un nom d'utilisateur à une entité. Il est essentiel pour les entités représentant des joueurs ou des personnages dans un environnement multijoueur.

## Attributs

- **username** : Une chaîne de caractères représentant le nom d'utilisateur ou l'identifiant unique.

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

- Identifier de manière unique des entités dans le jeu.
- Faciliter les interactions entre joueurs ou entre joueurs et serveurs.
- Synchroniser les actions et états des entités avec leurs identifiants.

## Exemples d'Utilisation

1. **Ajout à une entité** :
   ```cpp
   Entity &player = entityManager.createEntity();
   player.addComponent<UsernameComponent>("Player1");
   ```

2. **Récupération du nom d'utilisateur** :
   ```cpp
   auto *username = entity.getComponent<UsernameComponent>();
   if (username) {
       std::cout << "Nom d'utilisateur : " << username->username << std::endl;
   }
   ```

3. **Comparaison des identifiants** :
   ```cpp
   if (username->username == "Admin") {
       std::cout << "Accès administrateur accordé." << std::endl;
   }
   ```

## Interactions

- **Avec NetworkManager** : Utilisé pour identifier les messages ou actions associés à une entité spécifique.
- **Avec RenderSystem** : Affiche le nom d'utilisateur au-dessus des entités correspondantes.
- **Avec ScoreComponent** : Associe des scores ou statistiques spécifiques à un utilisateur.

## Fonctionnalités supplémentaires

- **Flexibilité d'identification** : Peut être utilisé avec n'importe quelle entité nécessitant un identifiant unique.
- **Support multijoueur** : Particulièrement utile dans des environnements en ligne pour gérer les interactions entre joueurs.

---

`UsernameComponent` est un composant simple mais crucial pour les systèmes nécessitant des identifiants uniques, en particulier dans des jeux multijoueurs ou collaboratifs.

