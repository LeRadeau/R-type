# PlayerReadyPacket

## Introduction

`PlayerReadyPacket` est utilisé pour signaler qu'un joueur est prêt à commencer la partie. Ce paquet est essentiel pour synchroniser les états de préparation des joueurs entre le serveur et les clients.

## Structure de la classe

### Attributs

- **playerId** : Identifiant unique du joueur prêt.

### Déclaration de la classe

```cpp
class PlayerReadyPacket : public Packet {
private:
    std::string playerId;

public:
    PlayerReadyPacket() = default;
    PlayerReadyPacket(const std::string &playerId)
        : playerId(playerId) {}

    void serialize(std::ostream &os) const override {
        os << playerId << ' ';
    }

    void deserialize(std::istream &is) override {
        is >> playerId;
    }

    PacketType getType() const override {
        return PacketType::PlayerReady;
    }

    const std::string &getPlayerId() const { return playerId; }
};
```

## Utilisation

### Envoi d'un PlayerReadyPacket

Le client envoie ce paquet au serveur lorsqu'il est prêt à commencer la partie.

```cpp
PlayerReadyPacket readyPacket("player1");
networkManager.sendPacket(readyPacket, serverIp, serverPort);
```

### Réception et traitement

Le serveur met à jour l'état des joueurs pour refléter leur préparation.

```cpp
if (packet->getType() == PacketType::PlayerReady) {
    auto readyPacket = dynamic_cast<PlayerReadyPacket *>(packet.get());
    gameState.markPlayerAsReady(readyPacket->getPlayerId());
    std::cout << "Joueur prêt : " << readyPacket->getPlayerId() << std::endl;
}
```

## Interactions

- **Avec GameState** : Met à jour l'état du joueur pour indiquer qu'il est prêt.
- **Avec GameStart** : Vérifie si tous les joueurs sont prêts avant de commencer la partie.

## Points forts

- **Synchronisation** : Garantit que le serveur connaît l'état de préparation de chaque joueur.
- **Flexibilité** : Facile à intégrer dans un flux multijoueur existant.
- **Clarté** : Fournit une interface simple pour signaler l'état de préparation.

---

`PlayerReadyPacket` est essentiel pour coordonner le début de la partie et s'assurer que tous les joueurs sont prêts avant le démarrage.

