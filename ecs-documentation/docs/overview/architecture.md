# Architecture de l'ECS (Entity Component System)

L'architecture de l'Entity Component System (ECS) mise en place dans ce projet vise à fournir une base modulaire et flexible pour le développement du jeu R-Type. Voici une vue d'ensemble de sa structure et de ses éléments principaux.

## Vue générale

L'ECS est composé de trois éléments principaux :

- **Entity (Entités)** : Représentent les objets du jeu.
- **Component (Composants)** : Fournissent les données associées à une entité.
- **System (Systèmes)** : Contiennent la logique qui manipule les composants des entités.

Cette architecture permet une forte découplage entre les données et la logique, ce qui rend le système facilement extensible et maintenable.

## Organisation du projet

Voici la structure du dossier ECS mise à jour :

```
client/ecs/
├── Entity.cpp
├── Entity.hpp
├── EntityManager.cpp
├── EntityManager.hpp
├── NetworkManager.cpp
├── NetworkManager.hpp
├── callback
│   ├── EventCallbacks.cpp
│   ├── EventCallbacks.hpp
│   ├── NetworkCallbacks.cpp
│   └── NetworkCallbacks.hpp
├── component
│   ├── AComponent.hpp
│   ├── BulletIdComponent.hpp
│   ├── EnnemyIdComponent.hpp
│   ├── EventHandlerComponent.hpp
│   ├── HealthComponent.hpp
│   ├── HoverComponent.hpp
│   ├── InputComponent.hpp
│   ├── NetworkCallbackComponent.hpp
│   ├── ParallaxComponent.hpp
│   ├── PositionComponent.hpp
│   ├── RectangleShapeComponent.hpp
│   ├── RenderComponent.hpp
│   ├── ScoreComponent.hpp
│   ├── SelectionComponent.hpp
│   ├── SoundComponent.hpp
│   ├── SpriteComponent.hpp
│   ├── TextComponent.hpp
│   ├── UsernameComponent.hpp
│   └── VelocityComponent.hpp
├── entity
│   ├── AllyEntity.cpp
│   ├── AllyEntity.hpp
│   ├── ButtonEntity.cpp
│   ├── ButtonEntity.hpp
│   ├── BydosEntity.cpp
│   ├── BydosEntity.hpp
│   ├── MenuEntity.cpp
│   ├── MenuEntity.hpp
│   ├── PlayerEntity.cpp
│   ├── PlayerEntity.hpp
│   ├── TextFieldEntity.cpp
│   └── TextFieldEntity.hpp
└── system
    ├── EventHandlingSystem.cpp
    ├── EventHandlingSystem.hpp
    ├── HoverSystem.cpp
    ├── HoverSystem.hpp
    ├── InputSystem.cpp
    ├── InputSystem.hpp
    ├── MessageSystem.cpp
    ├── MessageSystem.hpp
    ├── MovementSystem.cpp
    ├── MovementSystem.hpp
    ├── ParallaxSystem.cpp
    ├── ParallaxSystem.hpp
    ├── RenderSystem.cpp
    ├── RenderSystem.hpp
    ├── SelectionSystem.cpp
    ├── SelectionSystem.hpp
    ├── SoundSystem.cpp
    └── SoundSystem.hpp
```

### Entités et gestion

Les entités sont gérées par la classe **EntityManager**. Chaque entité possède un identifiant unique (ID) qui permet de les suivre et de les manipuler dans les différents systèmes. Parmi les entités notables :

- **AllyEntity** : Représente les alliés du joueur.
- **ButtonEntity** : Gère les boutons interactifs des interfaces utilisateur.
- **BydosEntity** : Représente les ennemis principaux.
- **MenuEntity** : Construit et gère les menus du jeu.
- **PlayerEntity** : Représente le joueur.
- **TextFieldEntity** : Gère les champs de texte interactifs.

### Composants

Les composants sont des structures de données pures, sans logique, qui décrivent les caractéristiques des entités. Ils sont organisés dans le dossier `component` et incluent des éléments tels que :

- **PositionComponent** : Gère la position spatiale d'une entité.
- **HealthComponent** : Suivi des points de vie.
- **RenderComponent** : Gestion de l'affichage.
- **HoverComponent** : Change l'apparence d'une entité lorsqu'elle est survolée.
- **SelectionComponent** : Indique si une entité est sélectionnée.
- **SoundComponent** : Gère les effets sonores associés à une entité.
- **ScoreComponent** : Enregistre et met à jour les scores des entités.

### Systèmes

Les systèmes implémentent la logique du jeu en interagissant avec les composants. Ils incluent des systèmes tels que :

- **InputSystem** : Gère les entrées utilisateur.
- **MovementSystem** : Met à jour les positions des entités en fonction des vitesses.
- **RenderSystem** : Dessine les entités à l'écran.
- **HoverSystem** : Modifie l'apparence des entités survolées.
- **SelectionSystem** : Gère la sélection des entités interactives.
- **SoundSystem** : Gère les effets sonores des entités.
- **ParallaxSystem** : Crée des effets de défilement parallaxe.
- **EventHandlingSystem** : Gère les interactions basées sur des événements utilisateur.
- **MessageSystem** : Synchronise les messages et les événements entre le client et le serveur.

### Gestion réseau

La classe **NetworkManager** gère les communications entre le client et le serveur, permettant de synchroniser l'état des entités à travers le réseau.

## Points forts de l'architecture

- **Flexibilité** : Facile à étendre avec de nouveaux composants ou systèmes.
- **Modularité** : Découplage des données et de la logique.
- **Performance** : Approprié pour un traitement efficace en boucle de jeu.

