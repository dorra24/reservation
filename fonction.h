#ifndef FONCTION_H_INCLUDED
#define FONCTION_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SALLE 10
#define MAX_RES 100

typedef enum {PENDING, CONFIRMED, CANCELLED} Status;

typedef struct {
    int id;
    char nom_client[50];
    char salle[50];
    char date[20];
    int heure_debut;
    int heure_fin;
    int nombre_personnes;
    float tarif;
    Status statut;
} Reservation;

typedef struct {
    char nom[50];
    int capacite;
    float tarif_horaire;
    char equipements[200];
} Salle;

typedef struct {
    int id;
    char nom[50];
    char email[100];
    char telephone[20];
} Client;

/* ---------- Fonctions principales ---------- */
int Disponibilite(char *nomSalle, char *date, int hDebut, int hFin, Reservation reservations[], int nb_res);
int verifCapacite(int nombre_personnes, Salle salles[], int nb_salles, char *nomSalle);
int trouverSalle(Salle salles[], int nb_salles, const char *nom);
int creerReservation(Reservation r, Reservation reservations[], int *nb_res, Salle salles[], int nb_salles);
float calculerTarifTotal(Reservation *r, Salle salles[], int nb_salles);
void genererFacture(Reservation r, const char *nomFichier);
void chiffredaffairesparsalle(Reservation reservations[], int nb_reservations);
void reservationsParMois(Reservation reservations[], int nb_reservations);
void sallesPopulaires(Reservation reservations[], int nb_reservations, Salle salles[], int nb_salles);
void rechercherReservationsClient(char *nom_client, Reservation reservations[], int nb_res);
float appliquerRemise(Reservation reservations[], int nb_res, Reservation *r);
int dateValide(const char *date);

/* ---------- Pile ---------- */
typedef struct {
    Reservation *tab[MAX_RES];
    int top;
} Pile;
void initPile(Pile *p);
int empiler(Pile *p, Reservation *r);
Reservation* depiler(Pile *p);
int pileVide(Pile *p);

/* ---------- File ---------- */
typedef struct {
    Reservation *tab[MAX_RES];
    int front;
    int rear;
    int taille;
} File;

void initFile(File *f);
int enfiler(File *f, Reservation *r);
Reservation* defiler(File *f);
int fileVide(File *f);



#endif // FONCTION_H_INCLUDED

