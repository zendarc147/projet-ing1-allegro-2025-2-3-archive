#include <allegro.h>
#include "moteur.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

void deplacement(t_perso *perso,t_plateau *plat, int x_end, int y_end){
    //on mets à jour les données
    perso->x = x_end;
    perso->y = y_end;

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