# Structure d'un ECS (Entity Component System)

**Note :** Cette structure est organisée sous forme de sous-dossiers pour une meilleure modularité et lisibilité.

## 1. Entities (Entités)
- Représentent les objets du jeu (par exemple, joueur, monstre, projectile).
- Ce sont des identifiants uniques sans logique ni données.
- **Dossier associé :** `Entities/`

## 2. Components (Composants)
- Contiennent les données associées aux entités.
- Chaque composant représente un aspect spécifique (position, vitesse, santé, etc.).
- **Dossier associé :** `Components/`

## 3. Systems (Systèmes)
- Gèrent la logique et le comportement du jeu.
- Manipulent les composants des entités pour appliquer des actions ou des règles 
  (exemple : mouvement, collision, rendu).
- **Dossier associé :** `Systems/`