# PlayerDisconnectPacket

## Introduction

`PlayerDisconnectPacket` est utilisé pour notifier la déconnexion d'un joueur. Ce paquet informe les clients et le serveur lorsqu'un joueur quitte volontairement ou est déconnecté pour d'autres raisons, garantissant que l'état du jeu reste synchronisé.

## Structure de la classe

### Attributs

- **playerId** : Identifiant unique du joueur déconnecté.

### Déclaration de la classe

```cpp
class PlayerDisconnectPacket : public Packet {
private:
    std::string playerId;

public:
    PlayerDisconnectPacket() = default;
    PlayerDisconnectPacket(const std::string &playerId)
        : playerId(playerId) {}

    void serialize(std::ostream &os) const override {
        os << playerId << ' ';
    }

    void deserialize(std::istream &is) override {
        is >> playerId;
    }

    PacketType getType() const override {
        return PacketType::PlayerDisconnect;
    }

    const std::string &getPlayerId() const { return playerId; }
};
```

## Utilisation

### Envoi d'un PlayerDisconnectPacket

Le client ou le serveur envoie ce paquet pour signaler une déconnexion.

```cpp
PlayerDisconnectPacket disconnectPacket("player1");
networkManager.sendPacket(disconnectPacket, serverIp, serverPort);
```

### Réception et traitement

Lorsqu'un client ou le serveur reçoit ce paquet, il met à jour l'état pour refléter la déconnexion.

```cpp
if (packet->getType() == PacketType::PlayerDisconnect) {
    auto disconnectPacket = dynamic_cast<PlayerDisconnectPacket *>(packet.get());
    gameState.removePlayer(disconnectPacket->getPlayerId());
    std::cout << "Joueur déconnecté : " << disconnectPacket->getPlayerId() << std::endl;
}
```

## Interactions

- **Avec GameState** : Supprime le joueur de l'état global du jeu.
- **Avec NetworkManager** : Sérialise et transmet les informations de déconnexion.

## Points forts

- **Synchronisation** : Garantit que tous les clients et le serveur sont informés des déconnexions.
- **Gestion simple** : Fournit une interface claire pour gérer les déconnexions.

---

`PlayerDisconnectPacket` est essentiel pour gérer efficacement les déconnexions, maintenant la cohérence entre les clients et le serveur.

