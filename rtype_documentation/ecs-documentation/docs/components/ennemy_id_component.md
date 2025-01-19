# EnnemyIdComponent

## Introduction

Le composant `EnnemyIdComponent` identifie chaque ennemi de manière unique dans le système. Il est utilisé pour distinguer les ennemis et suivre leur état dans le jeu.

## Attributs

- **id** : Une chaîne de caractères unique qui représente l'identifiant de l'ennemi.

### Exemple d'attributs dans le fichier `EnnemyIdComponent.hpp` :

```cpp
class EnnemyIdComponent : public AComponent {
public:
    std::string id;

    EnnemyIdComponent(const std::string &id) : id(id) {}
};
```

## Rôle dans l'ECS

`EnnemyIdComponent` est utilisé pour :

- Identifier chaque ennemi de manière unique dans le jeu.
- Faciliter la gestion des ennemis, comme le suivi de leur état ou l'application des dégâts.

## Exemples d'Utilisation

1. **Ajout à une entité** :
   ```cpp
   Entity &enemy = entityManager.createEntity();
   enemy.addComponent<EnnemyIdComponent>("enemy_001");
   ```

2. **Identification dans un système** :
   Lors de l'exécution d'un système, ce composant peut être utilisé pour identifier les ennemis :
   ```cpp
   auto *enemyId = entity->getComponent<EnnemyIdComponent>();
   if (enemyId) {
       std::cout << "Enemy ID : " << enemyId->id << std::endl;
   }
   ```

## Interactions

- **Avec HealthComponent** : Permet de gérer les points de vie de l'ennemi.
- **Avec PositionComponent** : Suit la position de l'ennemi sur la carte.
- **Avec RenderComponent** : Gère l'affichage de l'ennemi à l'écran.

---

Le composant `EnnemyIdComponent` est essentiel pour la gestion des ennemis dans le jeu, offrant un moyen simple de les identifier et de les manipuler.

