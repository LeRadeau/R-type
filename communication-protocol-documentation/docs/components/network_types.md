# Network Types

## Introduction

Le fichier `network_types.hpp` définit les types fondamentaux utilisés dans le protocole de communication. Ces types sont essentiels pour structurer et identifier les messages échangés entre le serveur et les clients.

## Types de messages

Les types de messages sont représentés par l'énumération `MessageType` :

```cpp
enum class MessageType : uint8_t {
    CONNECT,
    READY,
    START_GAME,
    MOVE,
    SHOOT,
    GOODBYE,
    BULLET_HIT,
    PLAYER_DEATH,
    GAME_OVER,
    UPDATE_CLIENTS,
    UPDATE_BULLETS,
    UPDATE_ENEMIES,
    ENEMY_DEATH,
    ERROR
};
```

### Description des messages

- **CONNECT** : Utilisé pour initier une connexion au serveur avec des informations telles que le nom d'utilisateur.
- **READY** : Indique que le client est prêt à commencer la partie.
- **START_GAME** : Signale le début du jeu.
- **MOVE** : Transmet les déplacements des joueurs.
- **SHOOT** : Envoie les données d'un tir effectué par un joueur.
- **GOODBYE** : Déconnecte un client.
- **BULLET_HIT** : Notifie qu'un projectile a touché une cible.
- **PLAYER_DEATH** : Informe qu'un joueur est mort.
- **GAME_OVER** : Indique que la partie est terminée.
- **UPDATE_CLIENTS** : Met à jour les informations sur les joueurs.
- **UPDATE_BULLETS** : Synchronise les données des projectiles.
- **UPDATE_ENEMIES** : Fournit les informations mises à jour des ennemis.
- **ENEMY_DEATH** : Informe de la mort d’un ennemi.
- **ERROR** : Transmet les messages d’erreur.

## Structures de données associées

Des structures additionnelles sont utilisées pour compléter les messages. Par exemple :

```cpp
struct ClientInfo {
    std::string username;
    sf::IpAddress ip;
    unsigned short port;
    sf::Vector2f position;
    int health;
    int score;
};

struct BulletInfo {
    std::string id;
    sf::Vector2f position;
    sf::Vector2f velocity;
};

struct EnemyInfo {
    std::string id;
    sf::Vector2f position;
    int health;
};
```

## Exemples d'utilisation

### Création d'un message `CONNECT`

```cpp
std::string buffer;
Serializer::serialize(buffer, static_cast<uint8_t>(MessageType::CONNECT));
Serializer::serialize(buffer, "Player1");
socket.send(buffer.data(), buffer.size(), serverIp, serverPort);
```

### Lecture d'un message `MOVE`

```cpp
const char *ptr = data;
auto messageType = static_cast<MessageType>(Serializer::deserialize<uint8_t>(ptr));
if (messageType == MessageType::MOVE) {
    auto playerId = Serializer::deserialize<std::string>(ptr);
    float x = Serializer::deserialize<float>(ptr);
    float y = Serializer::deserialize<float>(ptr);
    // Traiter les mouvements du joueur
}
```

---

Les types de messages définis dans `network_types.hpp` sont au cœur du protocole réseau, garantissant une communication claire et structurée entre les clients et le serveur.

