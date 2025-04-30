#include <stdlib.h>
#include "moteur.h"
#include <allegro.h>

void deplacement(t_perso perso, int plateau[20][20], int x_end, int y_end){
    install_mouse();
    int x_start= perso.x;
    int y_start= perso.y; // je prends les coordonnées du perso
    //dessiner rond
    int c = makecol(255,0,0);//pas utile pour la suite
    //implementer une fonction qui va faire le chemin le plus court
    circlefill(screen, x_start, y_start, 40, c);
    if (mouse_b & 1){
    x_end= mouse_x;//endroit souris cliquée
    y_end = mouse_y; //endroit souris cliquée
    circlefill(screen, x_end, y_end, 40, c); //dessiner rond
    }
}
/*
 *if (mouse cliquée){
 *deplacement
 *}*/