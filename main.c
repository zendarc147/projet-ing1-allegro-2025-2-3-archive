

#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro.h>
#include "moteur.h"

Button boutons[3];//les trois boutons
BITMAP *buffer; //le buffer
BITMAP *fond_ecran;

void initialisation_allegro() {
    allegro_init();
    install_keyboard();
    install_mouse();
    set_color_depth(desktop_color_depth());
    if(set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0) != 0) {
        allegro_message("probleme mode graphique");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
}

//Fonction pour dessiner un bouton
void dessiner_bouton(BITMAP *bmp, Button b) {
    int survole = (mouse_x > b.x && mouse_x < b.x + b.width && mouse_y > b.y && mouse_y < b.y + b.height);//on vérifie que la soutris survole le bouton

    int offset = (survole) ? 5 : 0; //si souris sur bouton, bouton s'aggrandit de 5 pixels
    rect(bmp, b.x - offset, b.y - offset, b.x + b.width + offset, b.y + b.height + offset, makecol(255, 255, 255));//dessine  le contour blanc

    // Dessiner texte au centre (ajuster aussi)
    textout_centre_ex(bmp, font, b.label,(b.x + b.width/2),(b.y + b.height/2) - 4,makecol(255, 255, 255), -1);
}
void deplacement(t_perso personnage, int plateau[20][20], int x, int y){
  //perso à x, y
  //dessiner un rond au nouvel emplacement - souris cliquée sur la case
  }

int main() {
    initialisation_allegro();

    buffer = create_bitmap(640, 480);//création du buffer
    fond_ecran = load_bitmap("istockphoto-1370110270-612x612.bmp", NULL);
    if (!fond_ecran) {
        allegro_message("Erreur chargement fond d'écran !");
        allegro_exit();
        exit(EXIT_FAILURE);
}

    //définir les boutons
    int largeur = 150;
    int hauteur = 40;
    int espace = 20;
    int depart_y = 150;
    int centre_x = (640 - largeur) / 2;

    strcpy(boutons[0].label, "Jouer");
    boutons[0].x = centre_x;
    boutons[0].y = depart_y;
    boutons[0].width = largeur;
    boutons[0].height = hauteur;

    strcpy(boutons[1].label, "Rejouer");
    boutons[1].x = centre_x;
    boutons[1].y = depart_y + hauteur + espace;
    boutons[1].width = largeur;
    boutons[1].height = hauteur;

    strcpy(boutons[2].label, "Quitter");
    boutons[2].x = centre_x;
    boutons[2].y = depart_y + 2*(hauteur + espace);
    boutons[2].width = largeur;
    boutons[2].height = hauteur;

    show_mouse(NULL);//affiche souris dans le buffer au lieu du screen

    while (!key[KEY_ESC]) {
        stretch_blit(fond_ecran, buffer, 0, 0, fond_ecran->w, fond_ecran->h, 0, 0, 640, 480);//affiche fond ecran étiré

        textout_centre_ex(buffer, font, "ECE Arena", 640/2, 50, makecol(255, 255, 255), -1);//affiche nom jeu

        for (int i = 0; i < 3; i++) {
            dessiner_bouton(buffer, boutons[i]);//dessine les trois boutons
        }


        draw_sprite(buffer, mouse_sprite, mouse_x, mouse_y);//dessine souris sur buffer


        blit(buffer, screen, 0, 0, 0, 0, 640, 480);//évite clignotement

        if (mouse_b & 1) { //clic gauche
            for (int i = 0; i < 3; i++) {
                if (mouse_x > boutons[i].x && mouse_x < boutons[i].x + boutons[i].width && mouse_y > boutons[i].y && mouse_y < boutons[i].y + boutons[i].height) {

                    //actions selon le bouton cliqué
                    if (i == 0) {//jouer
                        allegro_message("MAP");
                    }
                    else if (i == 1) {//rejouer
                        allegro_message("Rejouer");
                    }
                    else if (i == 2) {//quitter
                        destroy_bitmap(buffer);
                        destroy_bitmap(fond_ecran);
                        allegro_exit();
                        return 0;
                    }
                }
            }

            while (mouse_b & 1) rest(10);
        }

        rest(20);
    }

    destroy_bitmap(buffer);//libére buffer
    destroy_bitmap(fond_ecran);
    allegro_exit();
    return 0;
}
END_OF_MAIN();
