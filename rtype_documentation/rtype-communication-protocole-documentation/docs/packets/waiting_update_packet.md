# WaitingUpdatePacket

## Introduction

`WaitingUpdatePacket` est utilisé pour informer les clients de l'état actuel de la salle d'attente dans un jeu multijoueur. Ce paquet transmet des informations comme le nombre de joueurs prêts et le nombre total de joueurs dans la partie.

## Structure de la classe

### Attributs

- **readyPlayers** : Nombre de joueurs prêts dans la salle d'attente.
- **totalPlayers** : Nombre total de joueurs connectés.

### Déclaration de la classe

```cpp
class WaitingUpdatePacket : public Packet {
private:
    size_t readyPlayers;
    size_t totalPlayers;

public:
    WaitingUpdatePacket() = default;
    WaitingUpdatePacket(size_t readyPlayers, size_t totalPlayers)
        : readyPlayers(readyPlayers), totalPlayers(totalPlayers) {}

    void serialize(std::ostream &os) const override {
        os << readyPlayers << ' ' << totalPlayers << ' ';
    }

    void deserialize(std::istream &is) override {
        is >> readyPlayers >> totalPlayers;
    }

    PacketType getType() const override {
        return PacketType::WaitingUpdate;
    }

    size_t getReadyPlayers() const { return readyPlayers; }
    size_t getTotalPlayers() const { return totalPlayers; }
};
```

## Utilisation

### Envoi d'un WaitingUpdatePacket

Le serveur envoie ce paquet pour informer les clients de l'état actuel de la salle d'attente.

```cpp
WaitingUpdatePacket updatePacket(3, 5);
networkManager.sendPacket(updatePacket, clientIp, clientPort);
```

### Réception et traitement

Les clients mettent à jour leur interface utilisateur pour afficher l'état de la salle d'attente.

```cpp
if (packet->getType() == PacketType::WaitingUpdate) {
    auto waitingPacket = dynamic_cast<WaitingUpdatePacket *>(packet.get());
    ui.updateLobbyState(waitingPacket->getReadyPlayers(), waitingPacket->getTotalPlayers());
    std::cout << "Salle d'attente : " << waitingPacket->getReadyPlayers()
              << "/" << waitingPacket->getTotalPlayers() << " joueurs prêts." << std::endl;
}
```

## Interactions

- **Avec UIManager** : Met à jour l'interface utilisateur pour afficher les informations de la salle d'attente.
- **Avec GameState** : Peut être utilisé pour valider si tous les joueurs sont prêts avant de commencer la partie.

## Points forts

- **Clarté** : Fournit des informations précises sur l'état de la salle d'attente.
- **Synchronisation** : Garantit que tous les clients ont une vue cohérente de l'état des joueurs.
- **Flexibilité** : Facilement extensible pour inclure des informations supplémentaires.

---

`WaitingUpdatePacket` est un composant clé pour coordonner le processus d'attente avant le début d'une partie, garantissant une expérience utilisateur fluide et organisée.

