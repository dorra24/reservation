# reservation
Software in C for managing room reservations
# Gestion de Réservations de Salles

Ce projet est une application en C permettant de gérer des **salles** et des **réservations** pour des clients. Il inclut la gestion des disponibilités, des capacités, des tarifs et des statistiques.


### Gestion des Salles
- Ajouter de nouvelles salles avec nom, capacité, tarif horaire et équipements.
- Afficher la liste des salles existantes.

### Gestion des Réservations
- Ajouter une réservation avec nom du client, salle, date, horaires et nombre de personnes.
- Vérifier automatiquement la **disponibilité** et la **capacité**.
- Calcul automatique du **tarif** et application d'une **remise** après un certain nombre de réservations.
- Afficher toutes les réservations ou rechercher par client.
- Générer une **facture** pour une réservation.
- Gestion des réservations avec **pile** (LIFO) et **file** (FIFO) pour manipulations supplémentaires.

### Statistiques
- Chiffre d'affaires par salle.
- Nombre de réservations par mois.
- Salles les plus populaires.

## Structures principales
-Salle : nom, capacité, tarif horaire, équipements.
-Reservation : id, client, salle, date, horaires, nombre de personnes, tarif, statut.
-Client : id, nom, email, téléphone.
-Pile et File : pour empiler/dépiler et enfiler/défiler les réservations.

