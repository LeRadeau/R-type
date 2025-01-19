# PlayerShootPacket

## Introduction

`PlayerShootPacket` est utilisé pour transmettre les informations relatives aux tirs effectués par un joueur. Ce paquet informe le serveur et les autres clients de l'origine, de la direction et de la puissance du tir.

## Structure de la classe

### Attributs

- **playerId** : Identifiant unique du joueur ayant tiré.
- **position** : Position initiale du tir.
- **direction** : Direction du tir (vecteur normalisé).

### Déclaration de la classe

```cpp
class PlayerShootPacket : public Packet {
private:
    std::string playerId;
    sf::Vector2f position;
    sf::Vector2f direction;

public:
    PlayerShootPacket() = default;
    PlayerShootPacket(const std::string &playerId, const sf::Vector2f &position, const sf::Vector2f &direction)
        : playerId(playerId), position(position), direction(direction) {}

    void serialize(std::ostream &os) const override {
        os << playerId << ' '
           << position.x << ' ' << position.y << ' '
           << direction.x << ' ' << direction.y << ' ';
    }

    void deserialize(std::istream &is) override {
        is >> playerId >> position.x >> position.y >> direction.x >> direction.y;
    }

    PacketType getType() const override {
        return PacketType::PlayerShoot;
    }

    const std::string &getPlayerId() const { return playerId; }
    const sf::Vector2f &getPosition() const { return position; }
    const sf::Vector2f &getDirection() const { return direction; }
};
```

## Utilisation

### Envoi d'un PlayerShootPacket

Le client envoie ce paquet lorsqu'un joueur effectue un tir.

```cpp
PlayerShootPacket shootPacket("player1", {100.0f, 200.0f}, {1.0f, 0.0f});
networkManager.sendPacket(shootPacket, serverIp, serverPort);
```

### Réception et traitement

Le serveur ou les autres clients utilisent les informations pour créer et afficher un projectile.

```cpp
if (packet->getType() == PacketType::PlayerShoot) {
    auto shootPacket = dynamic_cast<PlayerShootPacket *>(packet.get());
    gameState.createBullet(shootPacket->getPlayerId(), shootPacket->getPosition(), shootPacket->getDirection());
    std::cout << "Tir du joueur " << shootPacket->getPlayerId()
              << " depuis (" << shootPacket->getPosition().x << ", "
              << shootPacket->getPosition().y << ") dans la direction ("
              << shootPacket->getDirection().x << ", " << shootPacket->getDirection().y << ")" << std::endl;
}
```

## Interactions

- **Avec GameState** : Ajoute un nouveau projectile à l'état global.
- **Avec RenderSystem** : Affiche le projectile à sa position initiale.
- **Avec PhysicsSystem** : Calcule la trajectoire et la collision du projectile.

## Points forts

- **Synchronisation** : Garantit que les tirs des joueurs sont synchronisés entre tous les participants.
- **Flexibilité** : Permet de transmettre des informations supplémentaires sur le tir si nécessaire (ex. : puissance).
- **Clarté** : Interface simple pour gérer les actions de tir.

---

`PlayerShootPacket` est crucial pour synchroniser les actions offensives des joueurs, garantissant une expérience multijoueur cohérente et dynamique.

