# NetworkManager

## Introduction

Le composant `NetworkManager` est responsable de la gestion des communications réseau entre le client et le serveur. Il fournit des fonctionnalités pour l'envoi, la réception et la gestion des paquets réseau tout en garantissant une exécution fluide et sécurisée dans un environnement multi-threadé.

## Fonctionnalités principales

### Envoi de paquets

La méthode `sendPacket` permet d'envoyer un paquet réseau sérialisé au serveur ou à un client.

```cpp
void NetworkManager::sendPacket(const Packet &packet, const sf::IpAddress &ip, unsigned short port);
```

#### Exemple : Envoi d'un message de connexion

```cpp
PlayerConnectPacket connectPacket("Player1");
networkManager.sendPacket(connectPacket, serverIp, serverPort);
```

### Réception de paquets

`NetworkManager` écoute en continu les messages entrants et les place dans une file sécurisée (`TSQueue`) pour un traitement asynchrone.

```cpp
void NetworkManager::receivePackets();
```

#### Exemple :

```cpp
while (running) {
    networkManager.receivePackets();
    while (!messageQueue.empty()) {
        auto packet = messageQueue.pop_front();
        handlePacket(packet);
    }
}
```

### Gestion des erreurs réseau

`NetworkManager` inclut un mécanisme pour capturer et gérer les exceptions réseau via des classes dédiées dans `NetworkExceptions.hpp`.

```cpp
try {
    networkManager.sendPacket(packet, ip, port);
} catch (const NetworkException &e) {
    std::cerr << "Erreur réseau : " << e.what() << std::endl;
}
```

### Initialisation

`NetworkManager` configure les sockets et démarre les threads pour gérer les communications réseau.

```cpp
NetworkManager::NetworkManager(unsigned short port) {
    if (socket.bind(port) != sf::Socket::Done) {
        throw NetworkInitializationException("Impossible de lier le port");
    }
    receiverThread = std::thread(&NetworkManager::receivePackets, this);
}
```

## Interactions

- **Avec Packet** : Utilise les classes dérivées de `Packet` pour sérialiser et désérialiser les données réseau.
- **Avec TSQueue** : Gère les messages entrants via une file sécurisée.
- **Avec NetworkExceptions** : Capture et traite les erreurs réseau pour garantir la stabilité.

## Exemple de flux

1. **Envoi d'un paquet** :
   - Le client envoie un message `PlayerMovePacket` au serveur.
   ```cpp
   PlayerMovePacket movePacket("Player1", 100.0f, 200.0f);
   networkManager.sendPacket(movePacket, serverIp, serverPort);
   ```

2. **Réception d'un paquet** :
   - Le serveur répond avec un `PlayersUpdatePacket`.
   ```cpp
   auto packet = messageQueue.pop_front();
   if (packet.getType() == PacketType::PlayersUpdate) {
       auto playersUpdate = static_cast<PlayersUpdatePacket &>(packet);
       playersUpdate.applyToGameState(gameState);
   }
   ```

## Points forts

- **Multithreading** : Gère simultanément l'envoi et la réception de messages.
- **Flexibilité** : Compatible avec différents types de paquets définis dans `Packet`.
- **Gestion des erreurs** : Capture et logue les erreurs réseau sans interrompre le système.

---

`NetworkManager` est un composant essentiel pour garantir une communication fluide et fiable entre les clients et le serveur, offrant des outils puissants pour gérer efficacement les interactions réseau.

