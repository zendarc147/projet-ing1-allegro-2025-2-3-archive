#include <allegro.h>
#include "moteur.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

void gerer_attaques(t_perso *perso,int spell){
    switch (perso->classe) {
        //case 1:
    }
}

#include <stdlib.h>   // pour abs()
#include "moteur.h"   // pour t_perso et t_attaque

#include <stdlib.h>   // pour abs()
#include "moteur.h"   // pour t_perso et t_attaque

void gerer_mode(int mode, t_perso *perso, int spell, int plateau[20][20]) {
    int i, j;

    if (mode == 1) {
        int px    = perso->x;
        int py    = perso->y;
        int range = perso->pm;


        // Ajouter 100 dans chaque case à portée de déplacement
        for (int dx = -range; dx <= range; dx++) {
            for (int dy = -range; dy <= range; dy++) {
                if (abs(dx) + abs(dy) <= range) {
                    int x = px + dx, y = py + dy;
                    if (x >= 0 && x < 20 && y >= 0 && y < 20)
                        plateau[x][y] += 100;
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
                        plateau[x][y] += 200;
                }
            }
        }
    }
}
