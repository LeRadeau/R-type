# Serializer

## Introduction

Le composant `Serializer` est utilisé pour convertir les données en un format sérialisé pouvant être transmis sur le réseau et pour reconstruire ces données côté réception. Il joue un rôle crucial dans le protocole de communication, garantissant l'intégrité des données échangées entre les clients et le serveur.

## Fonctionnalités principales

### Méthodes de sérialisation

#### Ajouter des données au tampon de sortie

```cpp
static void serialize(std::string &buffer, const T &value)
```
Cette méthode écrit une valeur de type générique `T` dans une chaîne de caractères.

#### Exemple d'utilisation :

```cpp
std::string buffer;
Serializer::serialize(buffer, static_cast<uint8_t>(MessageType::MOVE));
Serializer::serialize(buffer, playerId);
Serializer::serialize(buffer, positionX);
Serializer::serialize(buffer, positionY);
```

### Méthodes de désérialisation

#### Lire des données depuis un tampon d'entrée

```cpp
static T deserialize(const char *&buffer)
```
Cette méthode lit une valeur de type générique `T` depuis un pointeur sur une chaîne de caractères et avance le pointeur après la lecture.

#### Exemple d'utilisation :

```cpp
const char *ptr = data;
auto messageType = static_cast<MessageType>(Serializer::deserialize<uint8_t>(ptr));
float positionX = Serializer::deserialize<float>(ptr);
float positionY = Serializer::deserialize<float>(ptr);
```

## Prise en charge des types courants

`Serializer` prend en charge divers types de données, notamment :

- **Types primitifs** : `int`, `float`, `uint8_t`, etc.
- **Chaînes de caractères** : `std::string`.
- **Structures** : Tout type structuré pour lequel des fonctions de sérialisation spécifiques sont définies.

## Exemple de flux complet

### Sérialisation

Un message de type `MOVE` avec un identifiant joueur et une position :

```cpp
std::string buffer;
Serializer::serialize(buffer, static_cast<uint8_t>(MessageType::MOVE));
Serializer::serialize(buffer, "Player1");
Serializer::serialize(buffer, 100.0f);
Serializer::serialize(buffer, 200.0f);
socket.send(buffer.data(), buffer.size(), serverIp, serverPort);
```

### Désérialisation

Lecture des données reçues :

```cpp
const char *ptr = data;
auto messageType = static_cast<MessageType>(Serializer::deserialize<uint8_t>(ptr));
if (messageType == MessageType::MOVE) {
    auto playerId = Serializer::deserialize<std::string>(ptr);
    float x = Serializer::deserialize<float>(ptr);
    float y = Serializer::deserialize<float>(ptr);
    // Traiter les mouvements du joueur
}
```

## Points forts

- **Flexibilité** : Peut sérialiser et désérialiser des données de différents types.
- **Fiabilité** : Garantit l'intégrité des données transmises sur le réseau.
- **Performance** : Conçu pour une utilisation efficace dans des environnements réseau en temps réel.

---

`Serializer` est un composant central du protocole réseau, garantissant que les données sont correctement formatées et interprétées des deux côtés de la communication.

