# NetworkManager

## Introduction

`NetworkManager` est un composant central côté client qui gère les communications réseau avec le serveur. Il est responsable de l'envoi des messages, de la réception des données et de leur mise en file pour un traitement asynchrone.

## Rôle dans le client

- **Envoi** : Transmet les messages du client vers le serveur.
- **Réception** : Lit les messages entrants du serveur.
- **File d'attente** : Gère une file sécurisée pour stocker les messages reçus en attente de traitement.

## Fonctionnalités principales

### Envoi de messages

`send` permet de transmettre des messages au serveur.

```cpp
void NetworkManager::send(const std::string &buffer) {
    socket.send(buffer.data(), buffer.size(), serverIp, serverPort);
}
```

#### Exemple : Envoi d’un message `CONNECT`

```cpp
std::string buffer;
Serializer::serialize(buffer, static_cast<uint8_t>(MessageType::CONNECT));
Serializer::serialize(buffer, "Player1");
networkManager.send(buffer);
```

### Réception des messages

Les messages reçus sont stockés dans une file d’attente sécurisée (`TSQueue`) pour un traitement ultérieur.

```cpp
void NetworkManager::receiveMessages() {
    char data[1024];
    std::size_t received;
    sf::IpAddress sender;
    unsigned short senderPort;

    while (running) {
        if (socket.receive(data, sizeof(data), received, sender, senderPort) == sf::Socket::Done) {
            std::lock_guard<std::mutex> lock(queueMutex);
            receivedMessages.push(std::string(data, received));
        }
    }
}
```

### Gestion de la file d'attente

La méthode `getReceivedMessages` permet d’accéder aux messages reçus.

```cpp
std::queue<std::string> &NetworkManager::getReceivedMessages() {
    std::lock_guard<std::mutex> lock(queueMutex);
    return receivedMessages;
}
```

## Interactions

- **Avec Serializer** : Sérialise les messages avant l’envoi et désérialise ceux reçus.
- **Avec MessageSystem** : Alimente les messages à traiter.
- **Avec TSQueue** : Gère la file sécurisée pour les messages.

## Exemple de flux

1. Le joueur effectue une action (ex. : tir).
2. `NetworkManager` sérialise les données et envoie un message au serveur.
3. Le serveur répond avec une mise à jour (`UPDATE_CLIENTS`).
4. `NetworkManager` stocke le message reçu dans `receivedMessages`.
5. `MessageSystem` traite le message.

## Points forts

- **Multithreading** : Permet la réception et l’envoi simultanés.
- **Asynchrone** : Réduit les blocages en utilisant une file sécurisée.
- **Simplicité** : Une interface claire pour les communications réseau.

---

`NetworkManager` est un composant essentiel pour garantir une communication fluide et robuste entre le client et le serveur dans un environnement multijoueur.

