 # Architecture du Serveur

## Introduction

Le serveur dans le projet R-Type est conçu pour gérer les interactions multijoueurs en temps réel. Il centralise la gestion des joueurs, la logique de jeu et la synchronisation des états entre les clients connectés. Cette section détaille l'architecture globale du serveur et ses principaux modules.

## Vue d'ensemble

Le serveur est organisé en plusieurs modules, chacun ayant une responsabilité spécifique :

1. **Gestion des connexions** : Traite les nouvelles connexions, les déconnexions et la validation des clients.
2. **Gestion des messages** : Réception et traitement des paquets envoyés par les clients.
3. **Mise à jour des états** : Maintient une vue cohérente des entités et de leurs attributs (position, santé, etc.).
4. **Diffusion des mises à jour** : Envoie des informations synchronisées à tous les clients.

## Structure des fichiers du serveur

```
server/
├── Bullet.hpp
├── Client.hpp
├── Enemy.hpp
├── Server.cpp
├── Server.hpp
├── broadcast.cpp
├── game_state.cpp
├── gameloop.cpp
├── handle.cpp
├── CMakeLists.txt
```

### Description des fichiers

- **`Server.cpp` / `Server.hpp`** : Point d'entrée principal, initialisation des sockets et des modules.
- **`broadcast.cpp`** : Envoie des mises à jour aux clients.
- **`game_state.cpp`** : Maintient l'état global des entités dans le jeu.
- **`gameloop.cpp`** : Implémente la boucle principale du jeu.
- **`handle.cpp`** : Traite les paquets reçus et applique les mises à jour correspondantes.
- **`Bullet.hpp`, `Client.hpp`, `Enemy.hpp`** : Définitions des entités principales gérées par le serveur.

## Fonctionnalités principales

### Initialisation du serveur

Le serveur configure les sockets, charge les ressources nécessaires, et démarre la boucle de jeu.

```cpp
void Server::start() {
    socket.bind(port);
    gameLoop.run();
}
```

### Gestion des connexions

Les nouvelles connexions sont acceptées et validées via `Client`.

```cpp
void Server::addClient(const sf::IpAddress &ip, unsigned short port) {
    clients.emplace_back(ip, port);
}
```

### Gestion des messages

Les paquets reçus sont traités dans un ordre déterminé par leur type.

```cpp
void handleMessage(const Packet &packet) {
    switch (packet.getType()) {
        case PacketType::PlayerMove:
            handleMovePacket(packet);
            break;
        case PacketType::PlayerShoot:
            handleShootPacket(packet);
            break;
        default:
            std::cerr << "Type de paquet inconnu." << std::endl;
    }
}
```

### Diffusion

Les mises à jour des entités sont envoyées à tous les clients connectés.

```cpp
void broadcastUpdates() {
    for (const auto &client : clients) {
        networkManager.sendPacket(updatePacket, client.getIp(), client.getPort());
    }
}
```

## Points forts de l'architecture

- **Modularité** : Les modules sont bien isolés, ce qui facilite la maintenance.
- **Scalabilité** : Le serveur peut gérer plusieurs clients simultanément.
- **Synchronisation efficace** : Garantit que tous les clients ont une vue cohérente de l'état du jeu.

---

L'architecture du serveur fournit une base robuste et extensible pour gérer un jeu multijoueur en temps réel.

