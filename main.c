#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fonction.h"

void menu_salles(Salle salles[], int *nb_salles) {
    int choix;
    char buf[16];

    do {
        printf("\n--- Menu Salles ---\n");
        printf("1. Ajouter Salle\n2. Afficher Salles\n3. Retour\nChoix : ");
        fgets(buf, sizeof(buf), stdin);
        choix = atoi(buf);

        switch(choix) {
            case 1: {
                if(*nb_salles >= MAX_SALLE) {
                    printf("Nombre max de salles atteint.\n");
                    break;
                }
                Salle s;
                printf("Nom de la salle : ");
                fgets(s.nom, 50, stdin);
                s.nom[strcspn(s.nom, "\n")] = 0;
                printf("Capacite : ");
                scanf("%d", &s.capacite);
                printf("Tarif horaire : ");
                scanf("%f", &s.tarif_horaire);
                getchar();
                printf("Equipements : ");
                fgets(s.equipements, 200, stdin);
                s.equipements[strcspn(s.equipements, "\n")] = 0;
                salles[*nb_salles] = s;
                (*nb_salles)++;
                printf("Salle ajoutee.\n");
                break;
            }
            case 2:
                printf("\n=== Liste des Salles ===\n");
                for(int i=0; i<*nb_salles; i++)
                    printf("%s | Capacite: %d | Tarif: %.2f | Equipements: %s\n",
                           salles[i].nom, salles[i].capacite, salles[i].tarif_horaire, salles[i].equipements);
                break;
            case 3: break;
            default: printf("Choix invalide.\n");
        }
    } while(choix != 3);
}

void menu_reservations(Reservation reservations[], int *nb_res, Salle salles[], int nb_salles) {
    int choix;
    char buf[16];
    static int compteur_id = 1;

    static Pile pile;
    static int pile_init = 0;
    if(!pile_init) { initPile(&pile); pile_init = 1; }

    static File file;
    static int file_init = 0;
    if(!file_init) { initFile(&file); file_init = 1; }

    static File liste_attente;
    static int attente_init = 0;
    if(!attente_init) { initFile(&liste_attente); attente_init = 1; }

    do {
        printf("\n--- Menu Reservations ---\n");
        printf("1. Ajouter reservation\n2. Afficher toutes les reservations\n");
        printf("3. Rechercher reservation par client\n4. Generer facture\n");
        printf("5. Depiler une reservation\n6. Defiler une reservation\n");
        printf("7. Afficher liste d'attente\n8. Retour\nChoix : ");
        fgets(buf, sizeof(buf), stdin);
        choix = atoi(buf);

        switch(choix) {
            case 1: {
                Reservation r;
                r.id = compteur_id++;
                printf("Nom client : "); fgets(r.nom_client,50,stdin); r.nom_client[strcspn(r.nom_client,"\n")]=0;
                printf("Salle : "); fgets(r.salle,50,stdin); r.salle[strcspn(r.salle,"\n")]=0;

                do {
                    printf("Date (YYYY-MM-DD) : ");
                    fgets(r.date,20,stdin);
                    r.date[strcspn(r.date,"\n")]=0;
                    if(!dateValide(r.date)) printf("Date invalide ! Reessayez.\n");
                } while(!dateValide(r.date));

                printf("Heure debut : "); scanf("%d",&r.heure_debut);
                printf("Heure fin : "); scanf("%d",&r.heure_fin);
                printf("Nombre personnes : "); scanf("%d",&r.nombre_personnes);
                getchar();
                r.statut = CONFIRMED;

                int code = creerReservation(r,reservations,nb_res,salles,nb_salles);

                if(code == 0) {
                    reservations[*nb_res-1].tarif = appliquerRemise(reservations, *nb_res, &reservations[*nb_res-1]);
                    printf("Reservation ajoutee. Tarif final: %.2f\n", reservations[*nb_res-1].tarif);
                    empiler(&pile, &reservations[*nb_res-1]);
                    enfiler(&file, &reservations[*nb_res-1]);
                } else if(code == 1) {
                    printf("Salle introuvable !\n");
                } else if(code == 2) {
                    printf("Capacite insuffisante !\n");
                } else if(code == 3) {
                    printf("Conflit horaire, ajoutee à la liste d'attente.\n");
                    enfiler(&liste_attente, &r);
                } else if(code == 4) {
                    printf("Heure invalide !\n");
                }

                break;
            }

            case 2:
                printf("\n=== Liste des Reservations ===\n");
                for(int i=0;i<*nb_res;i++)
                    printf("ID:%d | %s | Salle:%s | Date:%s | %d-%d | %d pers | Tarif:%.2f | Statut:%d\n",
                           reservations[i].id, reservations[i].nom_client, reservations[i].salle,
                           reservations[i].date, reservations[i].heure_debut, reservations[i].heure_fin,
                           reservations[i].nombre_personnes, reservations[i].tarif, reservations[i].statut);
                break;

            case 3: {
                char nom[50];
                printf("Nom du client : "); fgets(nom,50,stdin); nom[strcspn(nom,"\n")]=0;
                rechercherReservationsClient(nom,reservations,*nb_res);
                break;
            }

            case 4: {
                if(*nb_res > 0) {
                    char fichier[50];
                    sprintf(fichier, "facture_%d.txt", reservations[*nb_res - 1].id);
                    genererFacture(reservations[*nb_res - 1], fichier);
                    printf("Facture generee : %s\n", fichier);
                } else printf("Pas de reservation.\n");
                break;
            }

            case 5: {
                if(!pileVide(&pile)) {
                    Reservation *r = depiler(&pile);
                    printf("Depile : %s dans salle %s\n", r->nom_client,r->salle);
                } else printf("Pile vide.\n");
                break;
            }

            case 6: {
                if(!fileVide(&file)) {
                    Reservation *r = defiler(&file);
                    printf("Defile : %s dans salle %s\n", r->nom_client,r->salle);
                } else printf("File vide.\n");
                break;
            }

            case 7: {
                printf("\n=== Liste d'attente ===\n");
                int taille = liste_attente.taille;
                if(taille == 0) printf("Liste d'attente vide.\n");
                else {
                    for(int i=0; i<taille; i++) {
                        Reservation *r = defiler(&liste_attente);
                        printf("Client: %s | Salle: %s | Date: %s | %d-%d | %d pers\n",
                               r->nom_client, r->salle, r->date, r->heure_debut, r->heure_fin, r->nombre_personnes);
                        enfiler(&liste_attente, r);
                    }
                }
                break;
            }

            case 8: break;
            default: printf("Choix invalide.\n");
        }

        int taille_attente = liste_attente.taille;
        for(int i=0; i<taille_attente; i++) {
            Reservation *r = defiler(&liste_attente);
            int code = creerReservation(*r,reservations,nb_res,salles,nb_salles);
            if(code == 0) {
                reservations[*nb_res-1].tarif = appliquerRemise(reservations, *nb_res, &reservations[*nb_res-1]);
                printf("Liste d'attente : reservation validee pour %s dans salle %s\n", r->nom_client, r->salle);
                empiler(&pile, &reservations[*nb_res-1]);
                enfiler(&file, &reservations[*nb_res-1]);
            } else if(code == 3) {
                enfiler(&liste_attente, r);
            } else if(code == 1) {
                printf("Liste d'attente : salle introuvable pour %s\n", r->nom_client);
            } else if(code == 2) {
                printf("Liste d'attente : capacité insuffisante pour %s\n", r->nom_client);
            } else if(code == 4) {
                printf("Liste d'attente : heure invalide pour %s\n", r->nom_client);
            }
        }

    } while(choix!=8);
}

void menu_stats(Reservation reservations[], int nb_res, Salle salles[], int nb_salles) {
    int choix;
    char buf[16];
    do {
        printf("\n--- Statistiques ---\n");
        printf("1. Chiffre d'affaires par salle\n2. Nombre de reservations par mois\n3. Salles populaires\n4. Retour\nChoix : ");
        fgets(buf,sizeof(buf),stdin);
        choix = atoi(buf);
        switch(choix) {
            case 1: chiffredaffairesparsalle(reservations,nb_res); break;
            case 2: reservationsParMois(reservations,nb_res); break;
            case 3: sallesPopulaires(reservations,nb_res,salles,nb_salles); break;
            case 4: break;
            default: printf("Choix invalide.\n");
        }
    } while(choix!=4);
}

int main() {
    Salle salles[MAX_SALLE] = {
        {"A",50,100.0,"Projecteur,Wifi"},
        {"B",100,150.0,"Tableau blanc,Wifi"},
        {"C",150,180.0,"Television"},
        {"D",200,220.0,"Climatisation,Wifi"},
        {"E",250,250.0,"Climatisation,Wifi"},
        {"F",300,280.0,"Climatisation,Wifi"},
    };
    int nb_salles = 6;

    Reservation reservations[MAX_RES];
    int nb_res = 0;

    int choix;
    char buf[16];

    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Gestion des Salles\n2. Gestion des Reservations\n3. Statistiques\n4. Quitter\nChoix : ");
        fgets(buf,sizeof(buf),stdin);
        choix = atoi(buf);
        switch(choix) {
            case 1: menu_salles(salles,&nb_salles); break;
            case 2: menu_reservations(reservations,&nb_res,salles,nb_salles); break;
            case 3: menu_stats(reservations,nb_res,salles,nb_salles); break;
            case 4: printf("Au revoir !\n"); break;
            default: printf("Choix invalide.\n");
        }
    } while(choix!=4);

    return 0;
}






