# Packet

## Introduction

La classe `Packet` et ses dérivées représentent les unités fondamentales des messages échangés entre le client et le serveur dans le protocole de communication. Elle offre une interface standard pour sérialiser, désérialiser et transmettre des données spécifiques à travers le réseau.

## Fonctionnalités principales

### Structure de base

La classe `Packet` est une classe abstraite qui définit les opérations fondamentales :

```cpp
class Packet {
public:
    virtual ~Packet() = default;

    virtual void serialize(std::ostream &stream) const = 0;
    virtual void deserialize(std::istream &stream) = 0;
    virtual PacketType getType() const = 0;
};
```

- **`serialize`** : Convertit les données du paquet en un format binaire pour l’envoi.
- **`deserialize`** : Reconstruit les données depuis un flux binaire reçu.
- **`getType`** : Retourne le type du paquet, défini par l’énumération `PacketType`.

### Types de paquets

Les paquets sont catégorisés par leurs rôles dans la communication :

```cpp
enum class PacketType {
    PlayerConnect,
    PlayerMove,
    PlayerShoot,
    PlayersUpdate,
    GameStart,
    GameOver,
    Error,
    // ... autres types
};
```

Chaque type de paquet correspond à une classe dérivée spécifique implémentant la logique associée.

### Exemple de classe dérivée

#### `PlayerMovePacket`

```cpp
class PlayerMovePacket : public Packet {
public:
    std::string playerId;
    float x, y;

    void serialize(std::ostream &stream) const override {
        stream << playerId << ' ' << x << ' ' << y << ' ';
    }

    void deserialize(std::istream &stream) override {
        stream >> playerId >> x >> y;
    }

    PacketType getType() const override {
        return PacketType::PlayerMove;
    }
};
```

### Gestion de la sérialisation

Tous les paquets utilisent les flux standard (`std::ostream` et `std::istream`) pour sérialiser et désérialiser les données, garantissant une flexibilité maximale pour les transmissions.

#### Sérialisation générale

Un objet `Packet` peut être sérialisé comme suit :

```cpp
std::ostringstream buffer;
packet.serialize(buffer);
socket.send(buffer.str().data(), buffer.str().size(), serverIp, serverPort);
```

#### Désérialisation générale

À la réception d’un message :

```cpp
std::istringstream stream(receivedData);
packet.deserialize(stream);
```

## Intégration avec `NetworkManager`

`NetworkManager` utilise les paquets pour structurer les messages réseau.

#### Envoi d’un paquet

```cpp
PlayerMovePacket movePacket("Player1", 100.0f, 200.0f);
networkManager.sendPacket(movePacket, serverIp, serverPort);
```

#### Réception d’un paquet

```cpp
auto packet = messageQueue.pop_front();
if (packet.getType() == PacketType::PlayerMove) {
    auto movePacket = static_cast<PlayerMovePacket &>(packet);
    handlePlayerMove(movePacket);
}
```

## Points forts

- **Extensibilité** : Ajout facile de nouveaux types de paquets.
- **Modularité** : Chaque type de paquet encapsule sa propre logique.
- **Interopérabilité** : Utilisation de flux standards pour la sérialisation.

---

La classe `Packet` offre une base robuste pour structurer les communications réseau, garantissant une transmission cohérente et flexible des données entre les clients et le serveur.

