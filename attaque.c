#include <stdlib.h>
#include <time.h>

#include "moteur.h"

int attaque(t_perso* attaquant, t_perso* defenseur, int attaque) {
    if(attaquant->pa >= attaquant->attaque[attaque].cout) {
        attaquant->pa -= attaquant->attaque[attaque].cout;
        int max = attaquant->attaque[attaque].degats_max;
        int min = attaquant->attaque[attaque].degats_min;
        srand(time(NULL));
        if(rand() % 100 <= attaquant->attaque[attaque].pourcent_echec) {textout_centre_ex(screen, font, "ECHEC", SCREEN_W/2, SCREEN_H/2, makecol(255,0,0), -1);rest(1000);return 0;}
        int degat = min + rand()%max-min+1;
        defenseur->pv -= degat;
        return 1;
    }
    textout_centre_ex(screen, font, "PAS ASSEZ DE PA", SCREEN_W/2, SCREEN_H/2, makecol(255,0,0), -1);rest(1000);

}





