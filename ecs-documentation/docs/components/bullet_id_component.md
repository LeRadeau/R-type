# BulletIdComponent

## Introduction

Le composant `BulletIdComponent` identifie de manière unique chaque projectile dans le jeu. Il est essentiel pour gérer les interactions et le suivi des projectiles sur la carte.

## Attributs

- **id** : Une chaîne de caractères unique qui identifie un projectile spécifique.

### Exemple d'attributs dans le fichier `BulletIdComponent.hpp` :

```cpp
class BulletIdComponent : public AComponent {
public:
    std::string id;

    BulletIdComponent(const std::string &id) : id(id) {}
};
```

## Rôle dans l'ECS

`BulletIdComponent` est utilisé pour :

- Identifier chaque projectile de manière unique dans le système.
- Faciliter la gestion des collisions et des mises à jour d'état des projectiles.

## Exemples d'Utilisation

1. **Ajout à une entité** :
   ```cpp
   Entity &bullet = entityManager.createEntity();
   bullet.addComponent<BulletIdComponent>("bullet_1234");
   ```

2. **Vérification des collisions** :
   Lors d'une collision, ce composant peut être utilisé pour récupérer l'ID d'un projectile et appliquer les effets correspondants :
   ```cpp
   auto *bulletId = entity->getComponent<BulletIdComponent>();
   if (bulletId) {
       std::cout << "Projectile ID : " << bulletId->id << std::endl;
   }
   ```

## Interactions

- **Avec PositionComponent** : Permet de suivre la position du projectile.
- **Avec VelocityComponent** : Gère la trajectoire du projectile en fonction de sa vitesse.

---

Le composant `BulletIdComponent` joue un rôle crucial dans la gestion des projectiles, en offrant un identifiant unique pour chaque instance.

