# EntityManager

## Introduction

Le gestionnaire d'entités, `EntityManager`, est un composant central du système ECS. Il est responsable de la création, de la gestion et de la suppression des entités dans le jeu.

## Rôle dans l'ECS

`EntityManager` a pour rôle de :

- Créer des entités et leur attribuer un identifiant unique.
- Gérer les composants associés à chaque entité.
- Faciliter l'interaction entre les systèmes et les entités.
- Supprimer les entités marquées pour suppression.

## Fonctionnalités principales

### Méthode `createEntity`

Crée une nouvelle entité et la retourne.

#### Exemple de code

```cpp
Entity &EntityManager::createEntity() {
    entities.push_back(std::make_unique<Entity>());
    return *entities.back();
}
```

### Méthode `markForDeletion`

Marque une entité pour suppression future.

#### Exemple de code

```cpp
void EntityManager::markForDeletion(uint64_t id) {
    toBeDeleted.push_back(id);
}
```

### Méthode `destroyMarkedEntities`

Supprime les entités qui ont été marquées pour suppression.

#### Exemple de code

```cpp
void EntityManager::destroyMarkedEntities() {
    for (uint64_t id : toBeDeleted) {
        entities.erase(std::remove_if(entities.begin(), entities.end(),
                           [id](const auto &e) {
                               return e->getId() == id;
                           }),
            entities.end());
    }
    toBeDeleted.clear();
}
```

## Exemples d'Utilisation

1. **Création d'une entité** :
   ```cpp
   Entity &player = entityManager.createEntity();
   player.addComponent<PositionComponent>(100.0f, 200.0f);
   player.addComponent<HealthComponent>(100, 100);
   ```

2. **Suppression d'une entité** :
   ```cpp
   entityManager.markForDeletion(player.getId());
   entityManager.destroyMarkedEntities();
   ```

## Interactions

- **Avec les systèmes ECS** : Les systèmes accèdent aux entités via `EntityManager` pour exécuter leurs opérations.
- **Avec les composants** : Gère les composants associés à chaque entité.

## Points forts

- **Gestion centralisée** : Toutes les entités sont gérées au même endroit, ce qui facilite leur manipulation.
- **Flexibilité** : Permet d'ajouter ou de supprimer des entités dynamiquement au cours du jeu.

---

`EntityManager` est un élément crucial de l'architecture ECS, offrant une gestion efficace et centralisée des entités dans le jeu.

