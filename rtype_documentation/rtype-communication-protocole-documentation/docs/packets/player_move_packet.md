# PlayerMovePacket

## Introduction

`PlayerMovePacket` est utilisé pour transmettre les informations relatives aux déplacements des joueurs. Ce paquet permet de synchroniser les positions des joueurs entre le serveur et les clients en temps réel.

## Structure de la classe

### Attributs

- **playerId** : Identifiant unique du joueur.
- **position** : Position actuelle du joueur.
- **velocity** : Vitesse du joueur (optionnel, selon le contexte).

### Déclaration de la classe

```cpp
class PlayerMovePacket : public Packet {
private:
    std::string playerId;
    sf::Vector2f position;
    sf::Vector2f velocity;

public:
    PlayerMovePacket() = default;
    PlayerMovePacket(const std::string &playerId, const sf::Vector2f &position, const sf::Vector2f &velocity)
        : playerId(playerId), position(position), velocity(velocity) {}

    void serialize(std::ostream &os) const override {
        os << playerId << ' '
           << position.x << ' ' << position.y << ' '
           << velocity.x << ' ' << velocity.y << ' ';
    }

    void deserialize(std::istream &is) override {
        is >> playerId >> position.x >> position.y >> velocity.x >> velocity.y;
    }

    PacketType getType() const override {
        return PacketType::PlayerMove;
    }

    const std::string &getPlayerId() const { return playerId; }
    const sf::Vector2f &getPosition() const { return position; }
    const sf::Vector2f &getVelocity() const { return velocity; }
};
```

## Utilisation

### Envoi d'un PlayerMovePacket

Le client envoie ce paquet au serveur lorsqu'un joueur se déplace.

```cpp
PlayerMovePacket movePacket("player1", {100.0f, 200.0f}, {1.0f, 0.0f});
networkManager.sendPacket(movePacket, serverIp, serverPort);
```

### Réception et traitement

Le serveur ou les autres clients mettent à jour les positions des entités locales en fonction des données reçues.

```cpp
if (packet->getType() == PacketType::PlayerMove) {
    auto movePacket = dynamic_cast<PlayerMovePacket *>(packet.get());
    gameState.updatePlayerPosition(movePacket->getPlayerId(), movePacket->getPosition());
    std::cout << "Joueur " << movePacket->getPlayerId()
              << " déplacé en position (" << movePacket->getPosition().x << ", "
              << movePacket->getPosition().y << ")" << std::endl;
}
```

## Interactions

- **Avec GameState** : Met à jour la position et la vitesse des joueurs dans l'état global.
- **Avec RenderSystem** : Ajuste l'affichage des joueurs en fonction de leurs nouvelles positions.
- **Avec CollisionSystem** : Vérifie les collisions après le déplacement.

## Points forts

- **Synchronisation en temps réel** : Garantit que les déplacements des joueurs sont reflétés instantanément pour tous les participants.
- **Flexibilité** : Inclut des informations supplémentaires comme la vitesse pour un comportement plus dynamique.
- **Optimisation** : Conçu pour transmettre uniquement les informations nécessaires à chaque mise à jour.

---

`PlayerMovePacket` est essentiel pour gérer les déplacements des joueurs, assurant une expérience multijoueur fluide et immersive.

