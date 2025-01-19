# Introduction

Bienvenue dans la documentation de l'architecture ECS (Entity Component System) du projet R-Type. Cette documentation a pour but de fournir une vue claire et détaillée de la structure et du fonctionnement de l'ECS, une partie centrale de notre moteur de jeu.

## Qu'est-ce qu'un ECS ?

L'Entity Component System est un modèle architectural conçu pour fournir une approche modulaire et flexible à la création de jeux. Il repose sur trois piliers principaux :

- **Entités** : Des conteneurs d'identité, sans logique ou données.
- **Composants** : Des structures de données attachées aux entités pour décrire leurs caractéristiques.
- **Systèmes** : Des modules logiques qui opèrent sur des ensembles d'entités ayant des composants pertinents.

Cette approche s'éloigne des modèles traditionnels orientés objet en découpant clairement les données et la logique, ce qui facilite l'évolution du code et la réutilisation des composants.

## Objectifs de l'ECS dans ce projet

Notre ECS est conçu pour répondre aux besoins spécifiques du jeu R-Type, notamment :

1. **Flexibilité** : Faciliter l'ajout de nouvelles fonctionnalités.
2. **Performance** : Optimiser les traitements grâce à une architecture orientée sur les données.
3. **Modularité** : Permettre une séparation claire des responsabilités entre les différents systèmes.

## Contenu de cette documentation

Voici les sections principales que vous trouverez dans cette documentation :

- **Overview** : Une vue d'ensemble de l'architecture et des concepts fondamentaux.
- **Components** : Détails des différents composants implémentés.
- **Systems** : Explication des systèmes et de leurs interactions avec les composants.
- **Managers** : Présentation des gestionnaires principaux, comme l'EntityManager et le NetworkManager.

## Pourquoi cette documentation est importante

Une documentation bien structurée est essentielle pour :

- Permettre aux nouveaux développeurs de se familiariser rapidement avec le projet.
- Assurer une maintenance efficace du code.
- Faciliter les évolutions futures en fournissant une vue d'ensemble claire des interactions entre les différents éléments.
