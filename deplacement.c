#include <stdlib.h>
#include <time.h>
#include "moteur.h"
#include <allegro.h>

void deplacement(t_perso perso, int plateau[20][20], int x1, int y1){
    install_mouse();
    x1= perso.x;
    y1= perso.y; // je prends les coordonnées du perso
    //dessiner rond
    
    if (mouse_b & 1){
    x1= mouse_x;//endroit souris cliquée
    y1 = mouse_y; //endroit souris cliquée
        //dessiner rond
    }
}
