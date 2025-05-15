#include <allegro.h>
#include "moteur.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>


t_perso* initialisation(){
  t_perso *perso = malloc(sizeof *perso * 4);
  if (!perso) return NULL;
  for(int i = 0; i < 4; i++) {
    perso[i].x = 5;
    perso[i].y = 5;
    perso[i].joueur = i;
    perso[i].classe = i;
    perso[i].pv = 1;
    perso[i].pa = 0;
    perso[i].pm = 0;
    for(int j = 0; j < 4; j++) {
      perso[i].attaque[j] = *attaques[i][j];
    }

  }
  return perso;

}

t_plateau* initialisation_plateau() {
  t_plateau *plateau = malloc(sizeof *plateau);
  for(int i = 0; i < 20; i++) {
    for(int j = 0; j < 20; j++) {
      plateau->plateau[i][j] = 0;
    }
  }
  plateau->bitmap = NULL;
  plateau->x = 12;
  plateau->y = 12;
  plateau->dx = 0;
  plateau->dy = 50;
  plateau->taille = 100;
  return plateau;

}
