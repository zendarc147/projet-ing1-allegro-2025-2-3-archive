#include <allegro.h>
#include "moteur.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

void deplacement(t_perso *perso,t_plateau *plat, int x_end, int y_end){
    int x_start= perso->x;
    int y_start= perso->y;

    //on mets à jour les données
    perso->x = x_end;
    perso->y = y_end;

    rest(2000);
    //deplacer mec avec la fonction sinon juste redessiner mec
    BITMAP *personage = load_bitmap("images/jedi_perso.bmp", NULL);
    if (!personage) {
        allegro_message("Erreur chargement personnage !");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    // Erreur potentielle : x_end et y_end ne sont pas les coordonnées écran (pixels),
    // mais les indices dans la grille. Il faut multiplier par une taille si c'est en tuiles.
    draw_sprite(screen, personage, x_end * plat->taille, y_end * plat->taille);
}

void trouver_chemin(int x, int y, int distance,t_plateau *plat) {
    if (plat->plateau[x][y] == -1) { //-1 = trou
        return;
    }
    if (plat->plateau[x][y] != 0 && plat->plateau[x][y] <= distance) {//dans le cas où 1 est l'objectif et 0 = case libre
        return;
    }
    plat->plateau[x][y] = distance;

    trouver_chemin(x+1, y, distance+1,plat);
    trouver_chemin(x-1, y, distance+1,plat);
    trouver_chemin(x, y+1, distance+1,plat);
    trouver_chemin(x, y-1, distance+1,plat);
}