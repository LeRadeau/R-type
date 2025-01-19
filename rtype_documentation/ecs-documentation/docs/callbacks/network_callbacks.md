# NetworkCallbacks

## Introduction

`NetworkCallbacks` contient des fonctions utilitaires permettant de gérer les messages réseau reçus du serveur. Ces fonctions sont associées à des composants `NetworkCallbackComponent` et déclenchées en réponse à des types de messages spécifiques.

## Rôle

`NetworkCallbacks` permet de :

- Traiter les messages réseau entrants en mettant à jour les composants ou entités concernées.
- Faciliter la gestion des interactions entre le serveur et le client.
- Simplifier la logique des systèmes en centralisant les comportements liés au réseau.

## Fonctions principales

### onWaitUpdateClientNbr

Met à jour l'affichage du nombre de joueurs en attente dans un lobby.

```cpp
void onWaitUpdateClientNbr(const char *&packet, Entity &textField);
```

#### Code extrait de `NetworkCallbacks.cpp`

```cpp
void NetworkCallbacks::onWaitUpdateClientNbr(const char *&packet, Entity &textField)
{
    size_t nbPlayers = Serializer::deserialize<std::size_t>(packet);
    std::cout << nbPlayers << std::endl;
    auto *textComponent = textField.getComponent<TextComponent>();
    textComponent->data.setString(std::to_string(nbPlayers) + "/4 players");
}
```

#### Exemple d'utilisation

```cpp
networkCallbackComponent.setCallback(MessageType::WAIT, [](const char *&packet) {
    NetworkCallbacks::onWaitUpdateClientNbr(packet, textFieldEntity);
});
```

## Exemple de workflow

1. **Ajout d'un callback réseau à une entité** :
   ```cpp
   auto &networkCallback = entity.addComponent<NetworkCallbackComponent>();
   networkCallback.setCallback(MessageType::WAIT, [](const char *&packet) {
       NetworkCallbacks::onWaitUpdateClientNbr(packet, entity);
   });
   ```

2. **Mise à jour d'un message réseau reçu** :
   Lorsque le serveur envoie un message de type `WAIT`, le callback est déclenché et met à jour l'interface utilisateur pour afficher le nombre de joueurs.

## Fonctionnalités principales

- **Gestion centralisée des messages réseau** : Permet de regrouper la logique liée aux messages réseau dans un espace organisé et modulable.
- **Interactivité** : Facilite la synchronisation entre le client et le serveur, améliorant la réactivité du jeu.

---

`NetworkCallbacks` est un élément clé pour gérer les interactions réseau dans le jeu. Il offre une solution centralisée et extensible pour synchroniser l'état du jeu entre les clients et le serveur.

