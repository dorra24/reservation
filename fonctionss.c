#include "fonction.h"

/* ---------- Disponibilité ---------- */
int Disponibilite(char *nomSalle, char *date, int hDebut, int hFin, Reservation reservations[], int nb_res) {
    if(hFin <= hDebut) return 0;
    for(int i = 0; i < nb_res; i++) {
        if(strcmp(reservations[i].salle, nomSalle) == 0 &&
           strcmp(reservations[i].date, date) == 0) {
            if(hDebut < reservations[i].heure_fin && reservations[i].heure_debut < hFin)
                return 0;
        }
    }
    return 1;
}

/* ---------- Vérification capacité ---------- */
int verifCapacite(int nombre_personnes, Salle salles[], int nb_salles, char *nomSalle) {
    for(int i = 0; i < nb_salles; i++)
        if(strcmp(salles[i].nom, nomSalle) == 0)
            return nombre_personnes <= salles[i].capacite;
    return 0;
}

/* ---------- Trouver salle ---------- */
int trouverSalle(Salle salles[], int nb_salles, const char *nom) {
    for(int i = 0; i < nb_salles; i++)
        if(strcmp(salles[i].nom, nom) == 0) return i;
    return -1;
}

/* ---------- Création réservation ---------- */
int creerReservation(Reservation r, Reservation reservations[], int *nb_res,
                     Salle salles[], int nb_salles)
{
    int indexSalle = trouverSalle(salles, nb_salles, r.salle);
    if(indexSalle == -1) return 1;
    if(r.heure_fin <= r.heure_debut) return 4;
    if(r.nombre_personnes > salles[indexSalle].capacite) return 2;

    for(int i = 0; i < *nb_res; i++) {
        if(strcmp(reservations[i].salle, r.salle) == 0 &&
           strcmp(reservations[i].date, r.date) == 0) {
            int d1 = r.heure_debut, f1 = r.heure_fin;
            int d2 = reservations[i].heure_debut, f2 = reservations[i].heure_fin;
            if(!(f1 <= d2 || d1 >= f2)) return 3;
        }
    }

    r.tarif = salles[indexSalle].tarif_horaire * (r.heure_fin - r.heure_debut);
    reservations[*nb_res] = r;
    (*nb_res)++;
    return 0;
}

/* ---------- Calcul tarif ---------- */
float calculerTarifTotal(Reservation *r, Salle salles[], int nb_salles) {
    for(int i = 0; i < nb_salles; i++)
        if(strcmp(salles[i].nom, r->salle) == 0) {
            r->tarif = salles[i].tarif_horaire * (r->heure_fin - r->heure_debut);
            break;
        }
    return r->tarif;
}

/* ---------- Facture ---------- */
void genererFacture(Reservation r, const char *nomFichier) {
    FILE *f = fopen(nomFichier, "w");
    if(!f) return;
    fprintf(f, "Facture ID : %d\nClient : %s\nID Client : %d\nSalle : %s\nDate : %s\nHeure : %d-%d\nTarif : %.2f DT\n",
            r.id, r.nom_client, r.id_client, r.salle, r.date, r.heure_debut, r.heure_fin, r.tarif);
    fclose(f);
}

/* ---------- Statistiques ---------- */
void chiffredaffairesparsalle(Reservation reservations[], int nb_reservations) {
    float ca[10] = {0};
    char noms[10][50];
    int nb_salles_res = 0;
    for(int i = 0; i < nb_reservations; i++) {
        int exist = -1;
        for(int j = 0; j < nb_salles_res; j++)
            if(strcmp(noms[j], reservations[i].salle) == 0) exist = j;
        if(exist == -1) { strcpy(noms[nb_salles_res], reservations[i].salle); exist = nb_salles_res++; }
        ca[exist] += reservations[i].tarif;
    }
    for(int i = 0; i < nb_salles_res; i++)
        printf("Salle %s : %.2f DT\n", noms[i], ca[i]);
}


void reservationsParMois(Reservation reservations[], int nb_reservations) {
    int annee;
    printf("Saisir annee pour les statistiques : ");
    scanf("%d", &annee);
    getchar(); // nettoyer le buffer

    int mois_count[12] = {0};

    for(int i = 0; i < nb_reservations; i++) {
        int res_annee, mois;
        if(sscanf(reservations[i].date, "%d-%d-%*d", &res_annee, &mois) != 2) continue;
        if(res_annee != annee) continue; // filtrer par année
        if(mois >= 1 && mois <= 12) mois_count[mois-1]++;
    }

    printf("Réservations par mois pour l'année %d :\n", annee);
    for(int i = 0; i < 12; i++)
        printf("Mois %02d : %d réservation(s)\n", i+1, mois_count[i]);
}


void sallesPopulaires(Reservation reservations[], int nb_reservations, Salle salles[], int nb_salles) {
    int compte[10] = {0};
    for(int i = 0; i < nb_reservations; i++)
        for(int j = 0; j < nb_salles; j++)
            if(strcmp(reservations[i].salle, salles[j].nom) == 0) { compte[j]++; break; }

    int max = 0;
    for(int i = 0; i < nb_salles; i++) if(compte[i] > max) max = compte[i];
    for(int i = 0; i < nb_salles; i++)
        if(compte[i] == max && max>0)
            printf("Salle populaire : %s (%d réservations)\n", salles[i].nom, compte[i]);
}

/* ---------- Recherche client par ID ---------- */
void rechercherReservationsClientParID(int id_client, Reservation reservations[], int nb_res) {
    int trouve = 0;
    for(int i = 0; i < nb_res; i++) {
        if(reservations[i].id_client == id_client) {
            trouve = 1;
            printf("ID Reservation: %d | Client ID: %d | Salle: %s | Date: %s | %d-%d | Tarif: %.2f\n",
                   reservations[i].id, reservations[i].id_client, reservations[i].salle,
                   reservations[i].date, reservations[i].heure_debut, reservations[i].heure_fin,
                   reservations[i].tarif);
        }
    }
    if(!trouve) printf("Aucune réservation trouvee pour le client ID %d\n", id_client);
}

/* ---------- Remise par ID client ---------- */
float appliquerRemise(Reservation reservations[], int nb_res, int id_client, float tarif_initial) {
    int compteur=0;
    for(int i=0;i<nb_res;i++) if(reservations[i].id_client==id_client) compteur++;
    float tarif_final = tarif_initial;
    if(compteur>=5) {
        tarif_final = tarif_initial*0.9;
        printf("Remise appliquee pour client ID %d ! Nouveau tarif: %.2f DT\n", id_client, tarif_final);
    }
    return tarif_final;
}

/* ---------- Pile ---------- */
void initPile(Pile *p){p->top=-1;}
int empiler(Pile *p, Reservation *r){if(p->top>=MAX_RES-1) return 0; p->tab[++p->top]=r; return 1;}
Reservation* depiler(Pile *p){if(p->top==-1) return NULL; return p->tab[p->top--];}
int pileVide(Pile *p){return p->top==-1;}

/* ---------- File ---------- */
void initFile(File *f){f->front=0; f->rear=-1; f->taille=0;}
int enfiler(File *f, Reservation *r){if(f->taille>=MAX_RES) return 0; f->rear=(f->rear+1)%MAX_RES; f->tab[f->rear]=r; f->taille++; return 1;}
Reservation* defiler(File *f){if(f->taille==0) return NULL; Reservation *r=f->tab[f->front]; f->front=(f->front+1)%MAX_RES; f->taille--; return r;}
int fileVide(File *f){return f->taille==0;}

//controle date
int dateValide(const char *date){
    int annee, mois, jour;
    if(sscanf(date,"%d-%d-%d",&annee,&mois,&jour)!=3) return 0;
    if(mois<1||mois>12) return 0;
    if(jour<1||jour>31) return 0;
    if((mois==4||mois==6||mois==9||mois==11)&&jour>30) return 0;
    if(mois==2){
        int bissextile=(annee%4==0&&(annee%100!=0||annee%400==0));
        if((bissextile && jour>29) || (!bissextile && jour>28)) return 0;
    }
    return 1;
}
//recherche client par id
int trouverClient(Client clients[], int nb_clients, int id_client){
    for(int i=0;i<nb_clients;i++) if(clients[i].id_client==id_client) return i;
    return -1;
}
int idValide(int id) {
    return (id >= 10000000 && id <= 99999999);
}


