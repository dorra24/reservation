# reservation
Software in C for managing room reservations
# Gestion de Réservations de Salles

Ce projet est une application en C permettant de gérer des **salles**, des **clients** et des **réservations** ainsi qu’une liste d’attente via une pile et une file.

## Structures principales
- `Salle` 
- `Reservation`
- `Client` 
- `Pile` et `File` 

- ## ## Structure du projet
├── main.c
├── fonction.h
├── fonctionss.c
└── README.md

- ## ## Fonctionnalités principales

### ✔ Disponibilité
Vérifie automatiquement si une salle est libre à la date et aux heures demandées, afin d’éviter les conflits d’horaires.

### ✔ Capacité
Compare le nombre de personnes de la réservation avec la capacité maximale de la salle.

### ✔ Création de réservation
Lorsqu'une réservation est créée, plusieurs contrôles sont effectués :
- validation des heures (heure fin > heure début)
- vérification de la disponibilité
- vérification de la capacité
- contrôle de la date
- calcul automatique du tarif
- en cas de réservation valide mais impossible → ajout à la **file d’attente**

### ✔ Calcul du tarif total
Le tarif est automatiquement calculé :  
**tarif horaire × durée de réservation**

### ✔ Facture
Génération d’un fichier `.txt` contenant :
- les informations du client  
- la salle réservée  
- la date et l’horaire  
- le tarif total

### ✔ Statistiques
Plusieurs statistiques sont générées automatiquement :
- chiffre d’affaires par salle
- nombre de réservations par mois d'une année
- salle la plus populaire (max de réservations)

### ✔ Recherche client
Affiche toutes les réservations faites par un client donné(avec id).

### ✔ Remise
Une réduction de **10%** est appliquée automatiquement si un client dépasse **5 réservations**.
### ✔ liste d’attente (File)
La file (File) stocke les réservations lorqu'il ya un conflit horaire dans une file.


## Auteurs
Projet développé par **Dorra Zouari**, **Rania Hafsa** **Gatri Yomn**.

**Langage utilisé :** `C`

