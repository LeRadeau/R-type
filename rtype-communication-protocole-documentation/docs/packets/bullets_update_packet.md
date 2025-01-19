# BulletsUpdatePacket

## Introduction

`BulletsUpdatePacket` est utilisé pour synchroniser l'état de plusieurs projectiles entre le serveur et les clients. Ce paquet permet de transmettre les positions et vitesses actuelles de toutes les balles en jeu, garantissant une expérience multijoueur fluide.

## Structure de la classe

### Attributs

- **bulletData** : Une liste d'objets contenant les informations de chaque balle, incluant :
  - `bulletId` : Identifiant unique de la balle.
  - `position` : Position actuelle de la balle.
  - `velocity` : Vitesse de la balle.

### Déclaration de la classe

```cpp
class BulletsUpdatePacket : public Packet {
private:
    struct BulletInfo {
        std::string bulletId;
        sf::Vector2f position;
        sf::Vector2f velocity;
    };

    std::vector<BulletInfo> bulletData;

public:
    BulletsUpdatePacket() = default;

    void serialize(std::ostream &os) const override {
        os << bulletData.size() << ' ';
        for (const auto &bullet : bulletData) {
            os << bullet.bulletId << ' '
               << bullet.position.x << ' ' << bullet.position.y << ' '
               << bullet.velocity.x << ' ' << bullet.velocity.y << ' ';
        }
    }

    void deserialize(std::istream &is) override {
        size_t count;
        is >> count;
        bulletData.resize(count);
        for (auto &bullet : bulletData) {
            is >> bullet.bulletId >> bullet.position.x >> bullet.position.y
               >> bullet.velocity.x >> bullet.velocity.y;
        }
    }

    PacketType getType() const override {
        return PacketType::BulletsUpdate;
    }

    const std::vector<BulletInfo> &getBulletData() const { return bulletData; }
};
```

## Utilisation

### Envoi d'un BulletsUpdatePacket

Le serveur envoie cet objet pour informer les clients des positions actuelles des balles.

```cpp
BulletsUpdatePacket updatePacket;
updatePacket.addBullet("bullet1", {100.0f, 200.0f}, {1.0f, 0.0f});
networkManager.sendPacket(updatePacket, clientIp, clientPort);
```

### Réception et traitement

Les clients mettent à jour leurs entités locales avec les données reçues.

```cpp
if (packet->getType() == PacketType::BulletsUpdate) {
    auto updatePacket = dynamic_cast<BulletsUpdatePacket *>(packet.get());
    for (const auto &bullet : updatePacket->getBulletData()) {
        gameState.updateBullet(bullet.bulletId, bullet.position, bullet.velocity);
    }
}
```

## Interactions

- **Avec GameState** : Met à jour les positions des balles dans le monde.
- **Avec RenderSystem** : Affiche les balles à leurs nouvelles positions.

## Points forts

- **Optimisation** : Transmet les données de plusieurs balles en un seul paquet.
- **Synchronisation** : Maintient une cohérence entre les instances client et serveur.

---

`BulletsUpdatePacket` est essentiel pour gérer efficacement la synchronisation des projectiles dans un environnement multijoueur.

