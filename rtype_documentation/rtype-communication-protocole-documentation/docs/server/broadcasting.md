# Broadcasting

## Introduction

Le module de diffusion (Broadcasting) est une composante essentielle du serveur, responsable de la transmission des mises à jour de l'état du jeu à tous les clients connectés. Il garantit que chaque joueur dispose d'une vue cohérente des événements en cours.

## Fonctionnalités principales

### Diffusion des mises à jour

Ce module envoie les informations mises à jour, telles que les positions des joueurs, les projectiles ou l'état des ennemis, à tous les clients connectés.

```cpp
void broadcastUpdates() {
    for (const auto &client : clients) {
        std::string updateData = createUpdateData();
        networkManager.sendPacket(updateData, client.getIp(), client.getPort());
    }
}
```

### Diffusion d'événements

Les événements spécifiques, tels que la mort d'un joueur ou le début de la partie, sont diffusés via des paquets dédiés.

```cpp
void broadcastEvent(const Event &event) {
    EventPacket eventPacket(event);
    for (const auto &client : clients) {
        networkManager.sendPacket(eventPacket, client.getIp(), client.getPort());
    }
}
```

### Gestion des erreurs de diffusion

Si une diffusion échoue, le serveur peut gérer l'erreur en réessayant ou en enregistrant le problème.

```cpp
void sendWithRetry(const Packet &packet, const Client &client) {
    int retries = 3;
    while (retries-- > 0) {
        if (networkManager.sendPacket(packet, client.getIp(), client.getPort()) == sf::Socket::Done) {
            return;
        }
        std::cerr << "Échec de l'envoi au client : " << client.getIp() << std::endl;
    }
}
```

## Interactions

- **Avec GameState** : Accède aux données des entités pour créer les mises à jour.
- **Avec NetworkManager** : Gère l'envoi des paquets réseau.
- **Avec MessageSystem** : Détecte les événements nécessitant une diffusion.

## Exemple de flux

1. Un joueur envoie un `PlayerMovePacket`.
2. Le serveur met à jour la position dans `GameState`.
3. Un `PlayersUpdatePacket` est créé et envoyé à tous les clients via `broadcastUpdates`.
4. Les clients synchronisent leurs entités locales avec les nouvelles données.

## Points forts

- **Efficacité** : Combine les mises à jour dans un seul paquet pour réduire la surcharge réseau.
- **Fiabilité** : Inclut des mécanismes de gestion des erreurs pour assurer la livraison.
- **Extensibilité** : Facilement adaptable à de nouveaux types de mises à jour ou d'événements.

---

Le module de diffusion est essentiel pour maintenir une synchronisation cohérente et fournir une expérience multijoueur immersive.

 