# Network Exceptions

## Introduction

Le fichier `NetworkExceptions.hpp` définit un ensemble d'exceptions personnalisées utilisées pour gérer les erreurs réseau. Ces exceptions offrent une manière standardisée de capturer et de traiter les problèmes liés à la communication entre le client et le serveur, garantissant une meilleure robustesse du système.

## Principales classes d'exceptions

### NetworkException

La classe de base pour toutes les exceptions réseau.

#### Définition

```cpp
class NetworkException : public std::exception {
public:
    explicit NetworkException(const std::string &message) : _message(message) {}
    const char *what() const noexcept override {
        return _message.c_str();
    }

private:
    std::string _message;
};
```

#### Exemple d'utilisation

```cpp
try {
    throw NetworkException("Erreur réseau générique");
} catch (const NetworkException &e) {
    std::cerr << e.what() << std::endl;
}
```

### NetworkInitializationException

Une exception spécifique pour les erreurs d'initialisation réseau, par exemple, l'impossibilité de lier un port ou d'ouvrir un socket.

#### Définition

```cpp
class NetworkInitializationException : public NetworkException {
public:
    explicit NetworkInitializationException(const std::string &message)
        : NetworkException("Initialisation réseau échouée: " + message) {}
};
```

#### Exemple d'utilisation

```cpp
if (socket.bind(port) != sf::Socket::Done) {
    throw NetworkInitializationException("Impossible de lier le port " + std::to_string(port));
}
```

### NetworkTransmissionException

Une exception dédiée aux erreurs de transmission, comme l'échec d'envoi ou de réception de paquets.

#### Définition

```cpp
class NetworkTransmissionException : public NetworkException {
public:
    explicit NetworkTransmissionException(const std::string &message)
        : NetworkException("Erreur de transmission: " + message) {}
};
```

#### Exemple d'utilisation

```cpp
if (socket.send(data, size, ip, port) != sf::Socket::Done) {
    throw NetworkTransmissionException("Échec d'envoi vers " + ip.toString());
}
```

## Avantages

1. **Standardisation** : Toutes les erreurs réseau utilisent une interface commune, facilitant leur gestion.
2. **Clarté** : Les messages d'erreur incluent des détails spécifiques sur la nature du problème.
3. **Robustesse** : Les exceptions permettent au système de réagir correctement sans planter.

## Exemple de flux d'erreurs

1. Une tentative de connexion échoue.
2. Une exception `NetworkInitializationException` est levée.
3. L'erreur est capturée et loguée :

```cpp
try {
    networkManager.connect("127.0.0.1", 54000);
} catch (const NetworkInitializationException &e) {
    std::cerr << e.what() << std::endl;
}
```

---

`NetworkExceptions.hpp` fournit une base solide pour gérer les erreurs réseau, garantissant une meilleure stabilité et une meilleure résilience du système.

