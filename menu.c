
#include "moteur.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro.h>

void draw_bouton(Button *bouton,BITMAP *buffer) {

}


int menu() {
    const int largeur = 150;
    const int hauteur = 40;
    const int espace  = 20;
    const int depart_y = 150;
    const int centre_x = (640 - largeur) / 2;
    Button boutons;
    // Initialisation des labels et positions des 3 boutons
    strcpy(boutons[0].label, "Jouer");
    boutons[0].x = centre_x;
    boutons[0].y = depart_y;
    boutons[0].width  = largeur;
    boutons[0].height = hauteur;

    strcpy(boutons[1].label, "Rejouer");
    boutons[1].x = centre_x;
    boutons[1].y = depart_y + hauteur + espace;
    boutons[1].width  = largeur;
    boutons[1].height = hauteur;

    strcpy(boutons[2].label, "Quitter");
    boutons[2].x = centre_x;
    boutons[2].y = depart_y + 2*(hauteur + espace);
    boutons[2].width  = largeur;
    boutons[2].height = hauteur;

    // Affichage de la souris dans le buffer
    show_mouse(NULL);

    int MenuState = 0;
    switch (MenuState) {


}
