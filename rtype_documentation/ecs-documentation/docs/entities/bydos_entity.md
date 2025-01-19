# BydosEntity

## Introduction

`BydosEntity` représente les ennemis principaux dans le jeu R-Type. Ces entités possèdent des caractéristiques spécifiques, telles que leur position, leur sprite et leur état de santé, pour gérer leur comportement dans le jeu.

## Rôle dans l'ECS

`BydosEntity` est utilisé pour :

- Représenter les ennemis dans le jeu avec des propriétés spécifiques.
- Intégrer les composants nécessaires pour le rendu, la gestion des collisions et les interactions avec les joueurs.
- Gérer les données relatives à la santé et à l'identité unique des ennemis.

## Attributs

Les entités `BydosEntity` sont créées avec les composants suivants :

- **PositionComponent** : Définit la position de l'ennemi.
- **SpriteComponent** : Affiche un sprite représentant visuellement l'ennemi.
- **EnnemyIdComponent** : Identifie l'ennemi de manière unique.
- **HealthComponent** : Gère les points de vie de l'ennemi.

### Exemple de création

#### Code extrait de `BydosEntity.cpp` :

```cpp
Entity &BydosEntity::createBydos(EntityManager &entityManager, float x, float y, const std::string &id, int health)
{
    Entity &bydos = entityManager.createEntity();
    bydos.addComponent<PositionComponent>(x, y);
    bydos.addComponent<SpriteComponent>("assets/Bydos64x64.png", sf::Vector2f(1, 1));
    bydos.addComponent<EnnemyIdComponent>(id);
    bydos.addComponent<HealthComponent>(health);
    return bydos;
}
```

## Interactions

- **Avec MovementSystem** : Gère les déplacements des ennemis dans le jeu.
- **Avec RenderSystem** : Affiche les sprites des ennemis à l'écran.
- **Avec CollisionSystem** : Détecte les interactions avec les projectiles ou les joueurs.
- **Avec HealthComponent** : Permet de réduire les points de vie des ennemis en cas de collision ou de dégâts.

## Exemples d'Utilisation

1. **Création d'un ennemi** :
   ```cpp
   BydosEntity::createBydos(entityManager, 500.0f, 300.0f, "enemy_001", 100);
   ```

2. **Mise à jour de la position** :
   ```cpp
   auto *position = enemy.getComponent<PositionComponent>();
   if (position) {
       position->position.x -= 5.0f;
   }
   ```

3. **Réduction des points de vie** :
   ```cpp
   auto *health = enemy.getComponent<HealthComponent>();
   if (health) {
       health->health -= 20;
       if (health->health <= 0) {
           entityManager.markForDeletion(enemy.getId());
       }
   }
   ```

---

`BydosEntity` est une entité centrale pour implémenter les ennemis dans le jeu, avec des fonctionnalités adaptées pour les interactions et les comportements spécifiques à R-Type.

