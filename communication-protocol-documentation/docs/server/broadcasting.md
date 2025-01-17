# Broadcasting

## Introduction

Le module de diffusion (Broadcasting) est responsable de l'envoi des mises à jour du serveur à tous les clients connectés. Il garantit que chaque joueur reste synchronisé avec l'état global du jeu.

## Rôle dans le serveur

- **Diffusion des états** : Envoyer les mises à jour des entités (joueurs, ennemis, projectiles) à tous les clients.
- **Communication efficace** : Utiliser des méthodes optimisées pour minimiser la latence et l'utilisation de la bande passante.
- **Gestion des erreurs** : Réessayer ou signaler les échecs de transmission.

## Fonctionnalités principales

### Envoi des mises à jour

Le serveur envoie les informations nécessaires pour synchroniser l'état des entités entre les clients.

```cpp
void Broadcaster::sendUpdate(const GameState &gameState) {
    for (const auto &client : gameState.clients) {
        std::string buffer;
        Serializer::serialize(buffer, static_cast<uint8_t>(MessageType::UPDATE_CLIENTS));
        Serializer::serialize(buffer, gameState);

        socket.send(buffer.data(), buffer.size(), client.ip, client.port);
    }
}
```

### Diffusion des événements

Certains événements, comme un joueur qui tire ou un ennemi éliminé, sont immédiatement diffusés :

```cpp
void Broadcaster::broadcastEvent(const Event &event) {
    for (const auto &client : gameState.clients) {
        std::string buffer;
        Serializer::serialize(buffer, static_cast<uint8_t>(event.type));
        Serializer::serialize(buffer, event.data);

        socket.send(buffer.data(), buffer.size(), client.ip, client.port);
    }
}
```

### Gestion des erreurs

Lorsqu'un message échoue à être envoyé, le module peut :

1. **Réessayer** :
   ```cpp
   if (socket.send(buffer.data(), buffer.size(), client.ip, client.port) != sf::Socket::Done) {
       std::cerr << "Erreur lors de l'envoi au client " << client.id << std::endl;
   }
   ```

2. **Notifier le système** :
   Notifier les autres modules pour gérer la déconnexion ou l'échec.

## Interactions

- **Avec GameState** : Accède aux données des clients et des entités pour construire les mises à jour.
- **Avec Serializer** : Prépare les messages dans un format transmissible.
- **Avec Socket** : Envoie les données via le réseau.

## Exemple de flux complet

1. Un joueur effectue une action (ex. : tire un projectile).
2. Le serveur met à jour l'état du jeu.
3. Le module de diffusion envoie les mises à jour aux clients :
   - Position du projectile.
   - État des entités affectées.
4. Les clients reçoivent et appliquent les mises à jour.

## Optimisations

- **Diffusion conditionnelle** : N'envoyer que les données pertinentes pour chaque client.
- **Agrégation des messages** : Combiner plusieurs mises à jour en un seul envoi pour réduire la surcharge réseau.

---

Le module de diffusion garantit que les joueurs restent synchronisés avec l'état du jeu en temps réel, offrant une expérience multijoueur fluide et immersive.

