# Message Flow

## Introduction

Le flux des messages est essentiel pour garantir une communication fluide et efficace entre le client et le serveur dans un environnement multijoueur. Cette section décrit le cheminement des messages, depuis leur création jusqu'à leur traitement, en expliquant chaque étape du flux.

## Étapes du flux

### 1. Création d'un message

Lorsqu'un événement se produit côté client, un message est créé, sérialisé, et envoyé au serveur.

#### Exemple : Message `MOVE`

```cpp
std::string buffer;
Serializer::serialize(buffer, static_cast<uint8_t>(MessageType::MOVE));
Serializer::serialize(buffer, playerId);
Serializer::serialize(buffer, positionX);
Serializer::serialize(buffer, positionY);
networkManager.send(buffer);
```

### 2. Envoi au serveur

Le message est transmis via le socket UDP du client au serveur. Le serveur écoute constamment les messages entrants.

```cpp
socket.send(buffer.data(), buffer.size(), serverIp, serverPort);
```

### 3. Réception côté serveur

Le serveur reçoit le message et le place dans une file d'attente pour le traitement.

```cpp
char data[1024];
socket.receive(data, sizeof(data), received, senderIp, senderPort);
tsQueue.push_back(std::string(data, received));
```

### 4. Traitement du message serveur

Le serveur extrait le message de la file et le traite en fonction de son type.

#### Exemple : Traitement d'un message `MOVE`

```cpp
const char *ptr = message.data();
MessageType type = static_cast<MessageType>(Serializer::deserialize<uint8_t>(ptr));
if (type == MessageType::MOVE) {
    handleMove(ptr);
}
```

### 5. Diffusion aux clients

Après traitement, le serveur envoie des mises à jour à tous les clients connectés pour synchroniser l'état du jeu.

```cpp
for (const auto &client : clients) {
    socket.send(buffer.data(), buffer.size(), client.ip, client.port);
}
```

### 6. Réception côté client

Les clients reçoivent les messages diffusés par le serveur. Ces messages sont stockés dans une file pour être traités par `MessageSystem`.

```cpp
char data[1024];
socket.receive(data, sizeof(data), received, senderIp, senderPort);
tsQueue.push_back(std::string(data, received));
```

### 7. Mise à jour des entités

`MessageSystem` traite les messages reçus et met à jour les composants ou entités correspondants.

```cpp
if (messageType == MessageType::UPDATE_CLIENTS) {
    handleUpdateClients(ptr);
}
```

## Exemple complet : Déplacement d'un joueur

1. **Client** : Le joueur déplace son vaisseau → Message `MOVE` créé et envoyé au serveur.
2. **Serveur** : Reçoit le message `MOVE`, met à jour la position du joueur, et diffuse une mise à jour `UPDATE_CLIENTS`.
3. **Client** : Reçoit `UPDATE_CLIENTS`, met à jour les positions des entités, et affiche les changements à l'écran.

## Visualisation du flux

```plaintext
Client 1 -----> [Message: MOVE] -----> Serveur -----> [Message: UPDATE_CLIENTS] -----> Tous les clients
```

---

Le flux des messages assure une synchronisation cohérente et une interaction fluide entre les clients et le serveur dans le jeu multijoueur.

