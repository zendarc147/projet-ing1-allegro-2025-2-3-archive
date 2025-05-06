#include <stdlib.h>
#include "moteur.h"
#include <allegro.h>

void deplacement(t_perso perso, int plateau[20][20], int x_end, int y_end){
    int x_start= perso.x;
    int y_start= perso.y; // je prends les coordonnées du perso
    //le mec est déja dessiné donc pas besoin de le redessiner
    //on mets à jour les données
    //mettre à jour plateau - A FAIRE
    perso.x = x_end;
    perso.y = y_end;
    //deplacer mec avec la fonction sinon juste redessiner mec
    BITMAP *perso_bitmap = load_bitmap("images/jedi_perso.bmp", NULL);
    if (!perso_bitmap) {
        allegro_message("Erreur chargement personnage !");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    draw_sprite(screen,perso_bitmap, x_end, y_end);
}
//#include "entete.h"

short trouver_chemin(int modelisation[16], int coordonnee, int deplacement){
    short cases = 0;
    int *chemin;
    int chemin_inter[16];
    int i, variations[4] = { -1, 4, -4, 1 }, nombre_choix = 0, choix[16], coordonnee_inter;

    // Initialisation chemin[] //
    if (deplacement){
        chemin = chemin_inter;
        for (i=0 ; i<16 ; i++) { chemin[i] = modelisation[i]; }
    } else { chemin = modelisation; }
    /////////////////////////////

    choix[0] = coordonnee;
    chemin[coordonnee] = 1;

    do{
        coordonnee = choix[nombre_choix];
        nombre_choix --;
        for (i=0 ; i<4 ; i++){
            coordonnee_inter = coordonnee + variations[i];
            if (coordonnee_inter < 16 && coordonnee_inter >= 0 && (chemin[coordonnee_inter] > chemin[coordonnee]+1 || !chemin[coordonnee_inter]) && !(coordonnee%4 == i && !(i%3))) {
                if (chemin[coordonnee] <= deplacement || !deplacement){
                    nombre_choix ++;
                    choix[nombre_choix] = coordonnee_inter;
                }
                chemin[coordonnee_inter] = chemin[coordonnee]+1;
            }
        }
    } while (nombre_choix >= 0);
    if (deplacement) {
        for (i=0 ; i<16 ; i++){
            if (chemin[i] <= deplacement+1 && chemin[i] > 0) { cases |= 1<<i; }
        }
    }

    return cases;
}
