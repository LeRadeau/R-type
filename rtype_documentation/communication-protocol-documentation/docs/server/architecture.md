# Architecture du Serveur

## Introduction

Le serveur dans le projet R-Type est conçu pour gérer les interactions multijoueurs en temps réel. Il centralise les communications, synchronise les états des joueurs et gère la logique de jeu. Cette section décrit l'architecture globale du serveur, ses principaux composants, et leurs interactions.

## Vue d'ensemble

Le serveur est structuré en plusieurs modules :

1. **Game Loop** : Gère la logique et les mises à jour du jeu.
2. **Message Handling** : Traite les messages réseau entrants et sortants.
3. **Broadcasting** : Envoie des mises à jour aux clients.
4. **Gestion des états** : Maintient une vue cohérente des entités et de leurs états.

## Structure des fichiers du serveur

```
server/
├── Server.cpp
├── Server.hpp
├── broadcast.cpp
├── game_state.cpp
├── gameloop.cpp
├── handle.cpp
├── CMakeLists.txt
```

### Description des fichiers

- **`Server.cpp` / `Server.hpp`** : Point d'entrée principal pour le serveur. Configure et lance les différents modules.
- **`broadcast.cpp`** : Gère l'envoi des messages aux clients.
- **`game_state.cpp`** : Maintient l'état global du jeu, y compris les entités et leurs données.
- **`gameloop.cpp`** : Implémente la boucle principale du jeu.
- **`handle.cpp`** : Traite les messages reçus et applique les changements correspondants.

## Fonctionnalités principales

### Initialisation du serveur

Le serveur initialise les composants critiques, ouvre un socket pour écouter les connexions et configure la boucle de jeu.

```cpp
void Server::start() {
    socket.bind(port);
    gameLoop.run();
}
```

### Communication réseau

- **Réception** : Le serveur écoute les messages entrants à l'aide de `TSQueue`.
- **Envoi** : Les réponses et mises à jour sont envoyées via des appels à `broadcast`.

### Gestion des joueurs

Chaque client connecté est identifié par une adresse IP, un port, et un identifiant unique. Les données des joueurs sont stockées dans `game_state.cpp`.

## Points forts de l'architecture

- **Modularité** : Chaque module est indépendant, facilitant les mises à jour et la maintenance.
- **Conception multithreadée** : Le serveur peut gérer plusieurs connexions simultanément grâce à `TSQueue` et des threads dédiés.
- **Performance** : Les messages sont traités de manière asynchrone pour éviter les blocages.

---

L'architecture du serveur fournit une base robuste et extensible pour gérer un jeu multijoueur en temps réel.

