# Protocol Overview

## Introduction

Le protocole de communication du projet R-Type définit les règles pour échanger des données entre le serveur et les clients. Il repose sur des messages sérialisés pour transmettre des informations critiques, comme les positions des entités, les actions des joueurs, et les événements de jeu.

## Structure des messages

Chaque message suit une structure standard pour garantir une compréhension universelle entre les composants réseau :

1. **Type de message** : Un entier (`uint8_t`) représentant la catégorie du message (ex. : `START_GAME`, `MOVE`, `SHOOT`).
2. **Données associées** : Une séquence sérialisée contenant les informations spécifiques au type de message.

### Exemple de structure :

| Champ           | Type      | Description                          |
|-----------------|-----------|--------------------------------------|
| Type de message | `uint8_t` | Identifie le type de message.        |
| Données         | `variable`| Contient les informations du message.|

## Types de messages

Le protocole prend en charge plusieurs types de messages, définis dans `network_types.hpp` :

- **`MessageType::CONNECT`** : Utilisé pour initier une connexion client.
- **`MessageType::START_GAME`** : Signale le début de la partie.
- **`MessageType::MOVE`** : Transmet les mouvements des joueurs.
- **`MessageType::SHOOT`** : Indique qu'un joueur a tiré.
- **`MessageType::UPDATE_CLIENTS`** : Synchronise les informations des joueurs.
- **`MessageType::UPDATE_BULLETS`** : Met à jour les informations des projectiles.
- **`MessageType::ERROR`** : Signale une erreur.

## Sérialisation des messages

Le fichier `Serializer.hpp` fournit des outils pour convertir les données en un format transmissible sur le réseau et les reconstruire côté réception.

### Exemple de sérialisation :

```cpp
std::string buffer;
Serializer::serialize(buffer, static_cast<uint8_t>(MessageType::MOVE));
Serializer::serialize(buffer, playerId);
Serializer::serialize(buffer, positionX);
Serializer::serialize(buffer, positionY);
socket.send(buffer.data(), buffer.size(), serverIp, serverPort);
```

### Exemple de désérialisation :

```cpp
const char *ptr = data;
auto messageType = static_cast<MessageType>(Serializer::deserialize<uint8_t>(ptr));
auto playerId = Serializer::deserialize<std::string>(ptr);
float positionX = Serializer::deserialize<float>(ptr);
float positionY = Serializer::deserialize<float>(ptr);
```

## Flux de communication

### Initialisation
1. Le client envoie un message `CONNECT` avec son nom d'utilisateur.
2. Le serveur répond avec un état initial du jeu.

### Boucle de jeu
1. Les clients envoient régulièrement des messages `MOVE` et `SHOOT`.
2. Le serveur diffuse des messages `UPDATE_CLIENTS`, `UPDATE_BULLETS` et `UPDATE_ENEMIES` pour synchroniser les états.

### Gestion des erreurs
1. Le serveur envoie des messages `ERROR` en cas de problème (ex. : collision de noms, serveur plein).

---

Cette vue d'ensemble présente les bases du protocole. Les sections suivantes détailleront les composants, la logique serveur, et les interactions client-serveur.

