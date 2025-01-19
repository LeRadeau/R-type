# Game Loop

## Introduction

La boucle de jeu (Game Loop) est le cœur du serveur dans le projet R-Type. Elle est responsable de l'exécution continue des mises à jour, du traitement des messages, et de la synchronisation des états des entités avec les clients. Cette section décrit la structure et le fonctionnement de cette boucle.

## Rôle de la Game Loop

- **Traitement des messages** : Récupérer et gérer les messages réseau reçus des clients.
- **Mise à jour des entités** : Appliquer la logique du jeu aux entités (déplacements, collisions, etc.).
- **Diffusion** : Envoyer des mises à jour aux clients pour synchroniser les états.

## Fonctionnement

### Initialisation

La boucle de jeu est configurée et lancée via la méthode `run` :

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

1. **Récupération des messages**
   - Les messages entrants sont extraits de la file sécurisée (`TSQueue`) et traités.
   - Exemple :
     ```cpp
     auto message = messageQueue.pop_front();
     handleMessage(message);
     ```

2. **Mise à jour de l'état du jeu**
   - Les entités sont mises à jour en fonction des messages et des règles du jeu.
   - Exemple :
     ```cpp
     for (auto &entity : gameState.entities) {
         entity.update(deltaTime);
     }
     ```

3. **Diffusion des états**
   - Les mises à jour sont envoyées aux clients via le module de diffusion (`broadcast`).
   - Exemple :
     ```cpp
     broadcast.sendUpdates(gameState);
     ```

## Synchronisation et performance

- **Cadence fixe** : La boucle de jeu fonctionne à une cadence définie pour garantir une expérience cohérente (ex. : 60 FPS).
- **Multithreading** : Les messages réseau sont traités sur des threads distincts pour éviter les blocages.
- **Optimisation** : Le délai d'attente entre les itérations est ajusté pour respecter la cadence tout en utilisant efficacement les ressources.

## Exemple de flux

1. Un joueur envoie un message `MOVE` pour déplacer son vaisseau.
2. Le message est traité dans `processIncomingMessages`.
3. La position du vaisseau est mise à jour dans `updateGameState`.
4. Les nouvelles positions des entités sont diffusées aux clients dans `broadcastUpdates`.

---

La boucle de jeu est un élément clé pour maintenir une synchronisation fluide et une logique de jeu cohérente dans un environnement multijoueur en temps réel.

