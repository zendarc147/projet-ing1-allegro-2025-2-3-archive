#include <stdlib.h>
#include <time.h>

#include "moteur.h"

int attaque(t_perso* attaquant, t_perso* defenseur, int attaque) {
    int max = attaquant->attaque[attaque].degats_max;
    int min = attaquant->attaque[attaque].degats_min;
    srand(time(NULL));
    if(rand() % 100 <= attaquant->attaque[attaque].pourcent_echec) {return 0;}
    defenseur->pv -= min + rand()%max-min+1;
    return 1;
}




