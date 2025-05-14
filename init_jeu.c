#include <allegro.h>
#include "moteur.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>


t_perso* initialisation(){
  t_perso *perso = malloc(sizeof *perso * 4);
  if (!perso) return NULL;
  for(int i = 0; i < 4; i++) {
    perso[i].x = i;
    perso[i].y = i;
    perso[i].joueur = i;
    perso[i].classe = i;
    perso[i].pv = 1;
    perso[i].pa = 2;
    perso[i].pm = 3;
    for(int j = 0; j < 4; j++) {
      perso[i].attaque[j] = *attaques[i][j];
    }

  }
  return perso;

}

