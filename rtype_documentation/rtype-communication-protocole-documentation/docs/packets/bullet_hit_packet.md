# BulletHitPacket

## Introduction

`BulletHitPacket` est un paquet réseau utilisé pour signaler qu'une balle a touché une cible. Ce type de message est crucial pour synchroniser les états des entités affectées par des impacts de projectiles dans le jeu multijoueur.

## Structure de la classe

### Attributs

- **bulletId** : Identifiant unique de la balle qui a touché.
- **targetId** : Identifiant unique de la cible affectée.
- **damage** : Quantité de dégâts infligés.

### Déclaration de la classe

```cpp
class BulletHitPacket : public Packet {
private:
    std::string bulletId;
    std::string targetId;
    int damage;

public:
    BulletHitPacket() = default;
    BulletHitPacket(const std::string &bulletId, const std::string &targetId, int damage)
        : bulletId(bulletId), targetId(targetId), damage(damage) {}

    void serialize(std::ostream &os) const override {
        os << bulletId << ' ' << targetId << ' ' << damage << ' ';
    }

    void deserialize(std::istream &is) override {
        is >> bulletId >> targetId >> damage;
    }

    PacketType getType() const override {
        return PacketType::BulletHit;
    }

    const std::string &getBulletId() const { return bulletId; }
    const std::string &getTargetId() const { return targetId; }
    int getDamage() const { return damage; }
};
```

## Utilisation

### Envoi d'un BulletHitPacket

Lorsqu'une balle touche une cible, le client ou le serveur peut envoyer un `BulletHitPacket` pour informer les autres parties.

```cpp
BulletHitPacket hitPacket("bullet123", "target456", 50);
networkManager.sendPacket(hitPacket, serverIp, serverPort);
```

### Réception et traitement

Le serveur ou le client traite le paquet pour mettre à jour l'état des entités :

```cpp
if (packet->getType() == PacketType::BulletHit) {
    auto hitPacket = dynamic_cast<BulletHitPacket *>(packet.get());
    gameState.applyDamage(hitPacket->getTargetId(), hitPacket->getDamage());
    std::cout << "Balle " << hitPacket->getBulletId() << " a touché la cible "
              << hitPacket->getTargetId() << " avec " << hitPacket->getDamage() << " dégâts." << std::endl;
}
```

## Interactions

- **Avec GameState** : Met à jour les points de vie de la cible touchée.
- **Avec NetworkManager** : Sérialise et envoie les informations d'impact.

## Points forts

- **Précision** : Chaque impact est documenté avec un identifiant de balle et de cible.
- **Simplicité** : Facilite le suivi des événements liés aux impacts dans le jeu.

---

`BulletHitPacket` joue un rôle clé dans la synchronisation des interactions entre les projectiles et les entités, garantissant une expérience multijoueur cohérente et immersive.

