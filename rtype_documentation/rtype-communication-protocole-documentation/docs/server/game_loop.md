# Game Loop

## Introduction

La boucle de jeu (Game Loop) est le cœur du serveur. Elle est responsable de l'exécution continue des mises à jour, du traitement des messages reçus et de la synchronisation des états du jeu avec les clients connectés. Cette section décrit le fonctionnement de cette boucle et ses principales étapes.

## Fonctionnement

### Initialisation

La boucle de jeu est initialisée avec les configurations nécessaires, telles que les paramètres du réseau et l'état initial des entités.

```cpp
void GameLoop::initialize() {
    loadGameState();
    setupTimers();
}
```

### Étapes de la boucle

La méthode principale, `run`, gère le flux de la boucle de jeu. Elle s'exécute à une cadence définie (exemple : 60 FPS).

#### Code simplifié :

```cpp
void GameLoop::run() {
    while (running) {
        auto startTime = std::chrono::high_resolution_clock::now();

        processIncomingMessages();
        updateGameState();
        broadcastUpdates();

        auto endTime = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(frameDuration - (endTime - startTime));
    }
}
```

### Étapes principales

1. **Traitement des messages entrants** :
   - Les messages reçus via `NetworkManager` sont analysés et traités.
   ```cpp
   void processIncomingMessages() {
       while (!messageQueue.empty()) {
           auto message = messageQueue.pop_front();
           handleMessage(message);
       }
   }
   ```

2. **Mise à jour de l'état du jeu** :
   - Les entités (joueurs, ennemis, projectiles) sont mises à jour en fonction des règles du jeu.
   ```cpp
   void updateGameState(float deltaTime) {
       for (auto &entity : gameState.entities) {
           entity.update(deltaTime);
       }
   }
   ```

3. **Diffusion des mises à jour** :
   - Les modifications de l'état du jeu sont envoyées à tous les clients.
   ```cpp
   void broadcastUpdates() {
       for (const auto &client : clients) {
           networkManager.sendPacket(updatePacket, client.getIp(), client.getPort());
       }
   }
   ```

### Gestion du temps

La cadence est maintenue à l'aide de `std::this_thread::sleep_for`, en ajustant la durée de chaque itération pour respecter une fréquence fixe.

```cpp
std::this_thread::sleep_for(frameDuration - elapsedTime);
```

## Interactions

- **Avec NetworkManager** : Récupère les messages des clients et envoie des mises à jour.
- **Avec GameState** : Applique les règles du jeu pour maintenir un état cohérent.
- **Avec TimerSystem** : Gère les événements dépendant du temps, comme les spawns d'ennemis.

## Exemple de flux

1. Un joueur envoie un message `PlayerMovePacket`.
2. Le serveur met à jour la position du joueur dans `updateGameState`.
3. La nouvelle position est incluse dans un `PlayersUpdatePacket`.
4. Le paquet est diffusé à tous les clients via `broadcastUpdates`.

## Points forts

- **Cadence fixe** : Maintient une fréquence stable pour une expérience fluide.
- **Multitâche** : Gère simultanément les messages entrants, les mises à jour et les diffusions.
- **Extensibilité** : Facile à adapter pour intégrer de nouvelles fonctionnalités.

---

La boucle de jeu du serveur est un élément clé pour garantir la fluidité et la synchronisation dans un environnement multijoueur.

