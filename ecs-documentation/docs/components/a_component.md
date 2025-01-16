# AComponent

## Introduction

Le composant `AComponent` est une classe de base abstraite qui sert de fondation pour tous les autres composants dans l'ECS. Il fournit une interface commune que tous les composants doivent implémenter, assurant ainsi une structure cohérente au sein du système.

## Attributs

Ce composant n'a pas d'attributs spécifiques car il est conçu pour être une classe de base abstraite. Les composants qui en héritent ajouteront leurs propres attributs.

## Rôle dans l'ECS

`AComponent` est essentiel pour :

- **Uniformiser les composants** : Tous les composants partagent une interface commune.
- **Faciliter la gestion des composants** : Permet aux entités de stocker et manipuler des composants de manière polymorphique.

## Exemples d'Utilisation

Voici un exemple de définition d'un composant qui hérite de `AComponent` :

```cpp
#include "AComponent.hpp"

class PositionComponent : public AComponent {
public:
    float x, y;

    PositionComponent(float x, float y) : x(x), y(y) {}
};
```

Ce modèle montre comment des composants comme `PositionComponent` peuvent être construits à partir de `AComponent` pour ajouter leurs propres fonctionnalités et données.

---

Ce composant abstrait constitue la base de tous les autres et garantit une structure cohérente dans le système ECS.

