# TSQueue

## Introduction

`TSQueue` (Thread-Safe Queue) est une structure de données conçue pour gérer en toute sécurité des files d'attente dans un environnement multi-threadé. Elle est utilisée pour assurer une communication fluide entre les threads, notamment pour traiter les messages réseau.

## Fonctionnalités principales

### Ajouter un élément à la file

```cpp
void push_back(const T &item);
```
Ajoute un élément à la fin de la file.

#### Exemple :
```cpp
tsQueue.push_back(message);
```

### Récupérer et supprimer un élément de la tête

```cpp
T pop_front();
```
Récupère et supprime l'élément situé à la tête de la file. Si la file est vide, elle attend qu'un élément soit disponible.

#### Exemple :
```cpp
T message = tsQueue.pop_front();
```

### Vérifier si la file est vide

```cpp
bool empty() const;
```
Renvoie `true` si la file est vide.

#### Exemple :
```cpp
if (tsQueue.empty()) {
    std::cout << "Aucun message dans la file." << std::endl;
}
```

### Obtenir le nombre d'éléments dans la file

```cpp
size_t size() const;
```
Renvoie le nombre actuel d'éléments dans la file.

#### Exemple :
```cpp
std::cout << "Messages en attente : " << tsQueue.size() << std::endl;
```

## Synchronisation multi-threadée

`TSQueue` utilise des verrous (`std::mutex`) et des variables conditionnelles (`std::condition_variable`) pour gérer les accès concurrentiels.

### Verrouillage pour l'ajout d'éléments

Lorsqu'un élément est ajouté, la file notifie les threads en attente via `std::condition_variable`.

### Blocage sur une file vide

La méthode `pop_front` bloque le thread jusqu'à ce qu'un élément soit disponible.

```cpp
std::unique_lock<std::mutex> lock(muxQueue);
cvBlocking.wait(lock, [this]() { return !queue.empty(); });
```

## Exemples d'utilisation

### Producteur-consommateur

#### Producteur :
```cpp
void producer(TSQueue<std::string> &queue) {
    queue.push_back("Message 1");
    queue.push_back("Message 2");
}
```

#### Consommateur :
```cpp
void consumer(TSQueue<std::string> &queue) {
    while (!queue.empty()) {
        std::string message = queue.pop_front();
        std::cout << "Message consommé : " << message << std::endl;
    }
}
```

### Utilisation avec le réseau

`TSQueue` est souvent utilisée pour stocker les messages reçus :

```cpp
while (socket.receive(buffer)) {
    tsQueue.push_back(buffer);
}

// Traitement des messages
while (!tsQueue.empty()) {
    auto message = tsQueue.pop_front();
    handleMessage(message);
}
```

## Points forts

- **Sécurisé pour les threads** : Conçu pour éviter les conditions de concurrence.
- **Blocage conditionnel** : Les threads consommateurs attendent automatiquement que des données soient disponibles.
- **Facilité d'utilisation** : Offre une interface simple pour les opérations courantes sur les files.

---

`TSQueue` est un outil essentiel pour la gestion des communications réseau dans un environnement multi-threadé, garantissant une coordination fluide et sans blocages inutiles.

