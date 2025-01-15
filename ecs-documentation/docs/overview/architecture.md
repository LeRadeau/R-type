# Architecture de l'ECS (Entity Component System)

L'architecture de l'Entity Component System (ECS) mise en place dans ce projet vise à fournir une base modulaire et flexible pour le développement du jeu R-Type. Voici une vue d'ensemble de sa structure et de ses éléments principaux.

## Vue générale

L'ECS est composé de trois éléments principaux :

- **Entity (Entités)** : Représentent les objets du jeu.
- **Component (Composants)** : Fournissent les données associées à une entité.
- **System (Systèmes)** : Contiennent la logique qui manipule les composants des entités.

Cette architecture permet une forte découplage entre les données et la logique, ce qui rend le système facilement extensible et maintenable.

## Organisation du projet

Voici la structure du dossier ECS :

```
client/ecs/
├── Entity.cpp
├── Entity.hpp
├── EntityManager.cpp
├── EntityManager.hpp
├── NetworkManager.cpp
├── NetworkManager.hpp
├── component
│   ├── AComponent.hpp
│   ├── BulletIdComponent.hpp
│   ├── EnnemyIdComponent.hpp
│   ├── HealthComponent.hpp
│   ├── InputComponent.hpp
│   ├── NetworkComponent.hpp
│   ├── PositionComponent.hpp
│   ├── RenderComponent.hpp
│   ├── SpriteComponent.hpp
│   ├── UsernameComponent.hpp
│   └── VelocityComponent.hpp
└── system
    ├── InputSystem.cpp
    ├── InputSystem.hpp
    ├── MessageSystem.cpp
    ├── MessageSystem.hpp
    ├── MovementSystem.cpp
    ├── MovementSystem.hpp
    ├── RenderSystem.cpp
    └── RenderSystem.hpp
```

### Entités et gestion

Les entités sont gérées par la classe **EntityManager**. Chaque entité possède un identifiant unique (ID) qui permet de les suivre et de les manipuler dans les différents systèmes.

### Composants

Les composants sont des structures de données pures, sans logique, qui décrivent les caractéristiques des entités. Ils sont organisés dans le dossier `component` et incluent des éléments tels que :

- **PositionComponent** : Gère la position spatiale d'une entité.
- **HealthComponent** : Suivi des points de vie.
- **RenderComponent** : Gestion de l'affichage.

### Systèmes

Les systèmes implémentent la logique du jeu en interagissant avec les composants. Ils incluent des systèmes tels que :

- **InputSystem** : Gère les entrées utilisateur.
- **MovementSystem** : Met à jour les positions des entités en fonction des vitesses.
- **RenderSystem** : Dessine les entités à l'écran.

### Gestion réseau

La classe **NetworkManager** gère les communications entre le client et le serveur, permettant de synchroniser l'état des entités à travers le réseau.

<!-- ## Diagramme de l'architecture ECS

![Diagramme ECS](../assets/ecs_diagram.png)

Le diagramme ci-dessus illustre les interactions entre les entités, composants, et systèmes, ainsi que le rôle du gestionnaire d'entités. -->

## Points forts de l'architecture

- **Flexibilité** : Facile à étendre avec de nouveaux composants ou systèmes.
- **Modularité** : Découplage des données et de la logique.
- **Performance** : Approprié pour un traitement efficace en boucle de jeu.


