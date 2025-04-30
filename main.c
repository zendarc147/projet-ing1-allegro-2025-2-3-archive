
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

int main() {
    initialisation_allegro();

    buffer = create_bitmap(640, 480);//création du buffer
    fond_ecran = load_bitmap("data\\fond_noir.bmp", NULL);
    if (!fond_ecran) {
        allegro_message("Erreur chargement fond d'écran !");
        allegro_exit();
        exit(EXIT_FAILURE);
    }


}
END_OF_MAIN();
