#ifndef MOTEUR_DE_JEU
#define MOTEUR_DE_JEU


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro.h>

#define MAX_JOUEURS 4

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
    int pv,pa,pm;
    int ordre_mort;//-1 si vivant
    t_attaque attaque[4];
} t_perso;

extern t_perso joueurs[MAX_JOUEURS];
extern int nb_joueurs;
extern int joueur_actuel;

typedef struct {
    const char *name;
    int base_PV;
    int base_attack;
} class_info_t;

typedef struct plateau {
    BITMAP *bitmap;
    int plateau[20][20];
    int x, y, dx, dy; // x,y les dimensions du plateau et dx,dy le décalage par rapport à la fenètre
    float taille;
} t_plateau;

typedef struct {
    int x, y; //coordonnées
    int g, h, f; //g=déja parcouru, h=ce qui reste, f=g+h
    struct Noeud* parent; //pour retrouver le chemin
} t_noeud;

/// VARIABLES GLOBALES ///
extern BITMAP *buffer;
extern BITMAP *mask;
extern SAMPLE *jeu_music;
extern volatile int quitter;

/// FONCTIONS ///
void initialisation_allegro();
void dessiner_boutons();
int menu(t_plateau *plateau);
int *charger_plateau(const char nom_fichier);
void afficher_plateau(t_plateau plateau, t_perso *perso, int nb_joueurs,int joueur);
void afficher_mask(t_plateau plateau); //afficher la bitmap de couleur pour détecter le clic de la souris
void deplacement(t_perso *perso, t_plateau *plat, int x, int y);
int attaque(t_perso* attaquant, t_perso* defenseur, int attaque);
void tour_de_jeu(int joueur);
int jeu(t_perso *perso,t_plateau *plateau,int nb_joueur);
void printPlateau(int plateau[20][20], int x, int y);
void gerer_mode(int mode, t_perso *perso, int spell, int plateau[20][20]);//gerer les cases vertes et rouges (lie plateau et mvt et attaques)
t_perso* initialisation();
t_plateau* initialisation_plateau();
void creer_plateau(t_plateau *plateau);
int case_survolee(t_plateau plateau);
void coor_case(t_plateau plateau, int case_x, int case_y, int* x, int* y);//transformer coordonnées case en coordonnées écran
void afficher_perso(t_plateau plateau, int perso_classe, int case_x, int case_y);
void gerer_attaques(t_perso *attaquant,t_perso *perso,int nb_joueurs,int spell,int x,int y);
int ecran_fin(t_perso *perso, int nb_joueurs,t_plateau *plateau);
void animation_attaque(t_plateau plateau, int perso_classe, int case_x, int case_y,int nb_frames);

extern const t_attaque *attaques[4][4];

#endif //MOTEUR_H
