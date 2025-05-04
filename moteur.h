#ifndef MOTEUR_DE_JEU
#define MOTEUR_DE_JEU


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro.h>
/// STRUCTURES ///
typedef struct attaque {
    int portee[9][9];
    int cout;
    int degats_min, degats_max;
    int pourcent_echec;
} t_attaque;

typedef struct perso {
    int joueur; // de 1 a 4
    int classe; // de 1 à 4
    int x, y;
    int pv;
    t_attaque attaque[4];
} t_perso;

typedef struct classe {
    int num_classe;
    const char *nom;
} t_classe;

/// VARIABLES GLOBALES ///
extern BITMAP *buffer;

/// FONCTIONS ///
void initialisation_allegro();
void dessiner_boutons();
int menu();
int *charger_plateau(const char nom_fichier);
void afficher_plateau(int *plateau, int X, int Y);
void deplacement(t_perso personnage, int plateau[20][20], int x, int y);
int attaque(t_perso* attaquant, t_perso* defenseur, int attaque);
void tour_de_jeu(int joueur);

#endif //MOTEUR_H
