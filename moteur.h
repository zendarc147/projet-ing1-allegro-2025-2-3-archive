#ifndef MOTEUR_DE_JEU
#define MOTEUR_DE_JEU

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

//structure pour les boutons
typedef struct button{
    int x, y, width, height;
    char label[20];
} Button;
/// FONCTIONS ///
void menu();
int *charger_plateau(const char nom_fichier);
void afficher_plateau(int *plateau);
void deplacement(t_perso personnage, int plateau[20][20], int x, int y);
int attaque(t_perso* attaquant, t_perso* defenseur, int attaque);
void tour_de_jeu(int joueur);
#endif //MOTEUR_H
