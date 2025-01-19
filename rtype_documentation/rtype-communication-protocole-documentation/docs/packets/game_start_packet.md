# GameStartPacket

## Introduction

`GameStartPacket` est utilisé pour signaler le début de la partie. Ce paquet est envoyé par le serveur aux clients une fois que tous les joueurs sont prêts, permettant de synchroniser le démarrage du jeu.

## Structure de la classe

### Attributs

Aucun attribut supplémentaire n'est nécessaire pour ce paquet, car il est simplement utilisé comme un signal pour indiquer le début de la partie.

### Déclaration de la classe

```cpp
class GameStartPacket : public Packet {
public:
    GameStartPacket() = default;

    void serialize(std::ostream &os) const override {
        // Aucune donnée supplémentaire à sérialiser
    }

    void deserialize(std::istream &is) override {
        // Aucune donnée supplémentaire à désérialiser
    }

    PacketType getType() const override {
        return PacketType::GameStart;
    }
};
```

## Utilisation

### Envoi d'un GameStartPacket

Le serveur envoie ce paquet lorsque tous les joueurs sont prêts à commencer la partie.

```cpp
GameStartPacket startPacket;
networkManager.sendPacket(startPacket, clientIp, clientPort);
```

### Réception et traitement

Les clients reçoivent ce signal et initient le démarrage du jeu localement.

```cpp
if (packet->getType() == PacketType::GameStart) {
    std::cout << "La partie commence maintenant !" << std::endl;
    gameState.startGame();
}
```

## Interactions

- **Avec GameState** : Change l'état du jeu pour indiquer qu'il est en cours.
- **Avec RenderSystem** : Déclenche les animations ou transitions d'introduction.

## Points forts

- **Simplicité** : Facile à implémenter, car aucun attribut supplémentaire n'est requis.
- **Synchronisation** : Garantit que tous les clients commencent la partie simultanément.

---

`GameStartPacket` est un élément simple mais essentiel pour coordonner le début de la partie dans un environnement multijoueur.

