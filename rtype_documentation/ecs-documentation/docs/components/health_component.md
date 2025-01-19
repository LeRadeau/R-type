# HealthComponent

## Introduction

Le composant `HealthComponent` est utilisé pour gérer les points de vie des entités dans le jeu. Il permet de suivre l'état de santé des entités et de déterminer si elles sont vivantes ou doivent être supprimées.

## Attributs

- **health** : Un entier représentant les points de vie actuels de l'entité.
- **maxHealth** : Un entier représentant le nombre maximum de points de vie que l'entité peut avoir.

### Exemple d'attributs dans le fichier `HealthComponent.hpp` :

```cpp
class HealthComponent : public AComponent {
public:
    int health;
    int maxHealth;

    HealthComponent(int health, int maxHealth) : health(health), maxHealth(maxHealth) {}
};
```

## Rôle dans l'ECS

`HealthComponent` est utilisé pour :

- Suivre l'état de santé des entités, telles que les joueurs, ennemis ou objets destructibles.
- Déclencher des événements lorsque la santé atteint 0 (par exemple, destruction ou mort).
- Gérer les soins ou autres modifications des points de vie.

## Exemples d'Utilisation

1. **Ajout à une entité** :
   ```cpp
   Entity &player = entityManager.createEntity();
   player.addComponent<HealthComponent>(100, 100);
   ```

2. **Réduction des points de vie** :
   ```cpp
   auto *health = entity->getComponent<HealthComponent>();
   if (health) {
       health->health -= 10;
       if (health->health <= 0) {
           entityManager.markForDeletion(entity->getId());
       }
   }
   ```

3. **Soin d'une entité** :
   ```cpp
   if (health && health->health < health->maxHealth) {
       health->health += 20;
       if (health->health > health->maxHealth) {
           health->health = health->maxHealth;
       }
   }
   ```

## Interactions

- **Avec RenderComponent** : Pour afficher une barre de vie ou d'autres indicateurs visuels.
- **Avec MessageSystem** : Pour synchroniser l'état de santé entre les clients et le serveur.

---

Le composant `HealthComponent` est un élément clé pour gérer l'état des entités dans le jeu, permettant des interactions dynamiques et réactives basées sur leur santé.

