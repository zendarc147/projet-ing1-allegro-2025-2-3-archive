#include "moteur.h"

void afficher_plateau(int *plateau, int X, int Y) {
    int x = 0, minXY = X < Y ? X : Y;
    float size = 6.0 / (X + Y);
    size = size > 1.0 ? size : 1.0;
    BITMAP *cases = load_bitmap("images/plateau/case.bmp", NULL);
    if (!cases) {
        printf("Erreur chargement image\n");
        exit(1);
    }

    for (int y = 0; y < X + Y; y++) {
        for (int i = 0; i < y && i < minXY && i < X + Y - y; i++) {
            stretch_sprite(buffer, cases, (x - 2 * i) * 100 * size, (y) * 62 * size, cases->w * size, cases->h * size);
        }
        y < X ? x++ : x--;
    }
    printf("Fin\n");
    blit(buffer, screen, 0, 0, 0, 0, 640, 480);
    while (!key[KEY_ESC]);

    destroy_bitmap(cases);
}

void afficher_donnes() { //int* nbr_joueurs,struct perso perso1,struct perso perso2,struct perso perso3,struct perso perso4
    for(int i = 0; i<5; i++) {
        rect(screen,50*i - 5,15,50*i +5,45,makecol(255,255,255));
        textout_centre_ex(buffer, font, "Perso X :", 50*i , 20, makecol(255,255,255), -1);
        textout_centre_ex(buffer, font, "Classe : ", 50*i, 30, makecol(255,255,255), -1);
        textout_centre_ex(buffer, font, "Pv : ", 50*i, 40, makecol(255,255,255), -1);
    }
    //partie 2 : perso actuel qui joue : classe , pv , pa , pm puis sorts du mec (tout change en fonction du perso
}
