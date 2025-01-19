# ErrorPacket

## Introduction

`ErrorPacket` est utilisé pour transmettre des informations sur des erreurs ou des anomalies détectées entre le client et le serveur. Ce paquet garantit une communication claire sur les problèmes rencontrés, permettant des réactions appropriées côté client ou serveur.

## Structure de la classe

### Attributs

- **errorCode** : Code unique représentant l'erreur.
- **errorMessage** : Message descriptif de l'erreur.

### Déclaration de la classe

```cpp
class ErrorPacket : public Packet {
private:
    int errorCode;
    std::string errorMessage;

public:
    ErrorPacket() = default;
    ErrorPacket(int errorCode, const std::string &errorMessage)
        : errorCode(errorCode), errorMessage(errorMessage) {}

    void serialize(std::ostream &os) const override {
        os << errorCode << ' ' << errorMessage << ' ';
    }

    void deserialize(std::istream &is) override {
        is >> errorCode;
        std::getline(is, errorMessage);
    }

    PacketType getType() const override {
        return PacketType::Error;
    }

    int getErrorCode() const { return errorCode; }
    const std::string &getErrorMessage() const { return errorMessage; }
};
```

## Utilisation

### Envoi d'un ErrorPacket

Le serveur envoie ce paquet pour notifier le client d'une erreur critique.

```cpp
ErrorPacket errorPacket(404, "Joueur non trouvé");
networkManager.sendPacket(errorPacket, clientIp, clientPort);
```

### Réception et traitement

Le client interprète les informations reçues et réagit en conséquence.

```cpp
if (packet->getType() == PacketType::Error) {
    auto errorPacket = dynamic_cast<ErrorPacket *>(packet.get());
    std::cerr << "Erreur reçue : [" << errorPacket->getErrorCode()
              << "] " << errorPacket->getErrorMessage() << std::endl;
}
```

## Interactions

- **Avec NetworkManager** : Envoie et reçoit les paquets d'erreur.
- **Avec Logger** : Logue les erreurs pour un suivi ultérieur.

## Points forts

- **Clarté** : Fournit des messages détaillés sur les erreurs pour faciliter le débogage.
- **Flexibilité** : Permet d'envoyer des codes d'erreur personnalisés adaptés à divers scénarios.
- **Communication proactive** : Informe les parties connectées des problèmes en temps réel.

---

`ErrorPacket` est un outil essentiel pour gérer les problèmes dans un environnement multijoueur, garantissant une communication efficace et une résolution rapide des anomalies.

