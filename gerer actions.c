#include <allegro.h>
#include "moteur.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

void gerer_attaques(t_perso *attaquant,t_perso *perso,int nb_joueurs,int spell,int x,int y){

    srand(time(NULL));
    t_perso *defenseur = NULL;
    for(int i=0;i<nb_joueurs;i++) {
        if(perso[i].x == x && perso[i].y == y) {defenseur = &perso[i];}
    }
    if(defenseur != NULL) {
        if(spell == 5) {
            defenseur->pv -= 10;
            return;
        }
        switch(attaquant->classe) {
            case 2:switch (spell) {
                case 0: attaque(attaquant,defenseur,spell);return;
                case 1: attaque(attaquant, defenseur, spell);return;
                case 2: attaque(attaquant, defenseur, spell);return;
                case 3: attaquant->pv += 10;return;
            }
            case 0:switch (spell) {
                case 0: attaque(attaquant,defenseur,spell); return;
                case 1: attaque(attaquant, defenseur, spell);return;
                case 2:
                    for(int i=0;i<6;i++) {
                        int col = rand()%12;
                        int row = rand()%12;
                        if(row == defenseur->x && col == defenseur->y)defenseur->pv -= attaquant->attaque[spell].degats_max;
                    }
                return;


                case 3:attaquant->pa += 3;return;
            }
            case 1:switch (spell) {
                case 0: attaque(attaquant,defenseur,spell); return;
                case 1: attaque(attaquant, defenseur, spell);return;
                case 2: attaque(attaquant, defenseur, spell);return;
                case 3:for(int i=0;i<4;i++) {
                    attaquant->attaque[i].degats_max += 5;
                    attaquant->attaque[i].degats_min += 5;
                }
                return;
            }
            case 3:switch (spell) {
                case 0: attaque(attaquant,defenseur,spell); return;
                case 1: attaque(attaquant, defenseur, spell);return;
                case 2: attaque(attaquant, defenseur, spell);return;
                case 3: attaquant->pm += 3;return;
            }

        }
    }

}

#include <stdlib.h>   // pour abs()
#include "moteur.h"   // pour t_perso et t_attaque

#include <stdlib.h>   // pour abs()
#include "moteur.h"   // pour t_perso et t_attaque

void gerer_mode(int mode, t_perso *perso, int spell, int plateau[20][20]) {
    int i, j;

        for (i = 0; i < 20; i++) {
            for (j = 0; j < 20; j++) {
                if (plateau[i][j] >= 100 && plateau[i][j] < 200) {plateau[i][j] -= 100;}
                else if (plateau[i][j] >= 200 && plateau[i][j] < 300) {plateau[i][j] -= 200;}
            }
        }


    if (mode == 1) {
        int px    = perso->x;
        int py    = perso->y;
        int range = perso->pm;



        for (int dx = -range; dx <= range; dx++) {
            for (int dy = -range; dy <= range; dy++) {
                if (abs(dx) + abs(dy) <= range) {
                    int x = px + dx, y = py + dy;
                    if (x >= 0 && x < 20 && y >= 0 && y < 20)
                        if(plateau[x][y]>=0){plateau[x][y] += 100;}
                }
            }
        }
    }
    else if (mode == 2) {
        int px = perso->x;
        int py = perso->y;
        t_attaque *atk = &perso->attaque[spell];


        // Ajouter 200 dans chaque case couverte par l'attaque
        for (i = 0; i < 9; i++) {
            for (j = 0; j < 9; j++) {
                if (atk->portee[i][j]) {
                    int dx = i - 4;
                    int dy = j - 4;
                    int x  = px + dx;
                    int y  = py + dy;
                    if (x >= 0 && x < 20 && y >= 0 && y < 20)
                        if(plateau[x][y]>=0){plateau[x][y] += 200;}
                }
            }
        }
    }

    if(mode == 3) {
        int px    = perso->x;
        int py = perso->y;
        if(px>=0 && py+1>=0 && px<20 && py+1<20) {if(plateau[px][py]>=0){plateau[px][py+1] += 200;}}
        if(px>=0 && py-1>=0 && px<20 && py-1<20) {if(plateau[px][py]>=0){plateau[px][py-1] += 200;}}
        if(px+1>=0 && py>=0 && px+1<20 && py<20) {if(plateau[px][py]>=0){plateau[px+1][py] += 200;}}
        if(px-1>=0 && py>=0 && px-1<20 && py<20) {if(plateau[px][py]>=0){plateau[px-1][py] += 200;}}

    }

}
