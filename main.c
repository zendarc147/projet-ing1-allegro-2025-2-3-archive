#include "moteur.h"

BITMAP *buffer = NULL;

t_perso *perso;


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
    buffer = create_bitmap(640, 480);//création du buffer
}

int main() {

    initialisation_allegro();
    t_perso *perso = initialisation();
    int plateau[20][20];
    for(int i = 0; i < 20; i++) {
        for(int j = 0; j < 20; j++) {
            plateau[i][j] = 0;
        }
    }
    //menu();
    jeu(perso,plateau);
    afficher_plateau(plateau,0 ,0);
    destroy_bitmap(buffer);//libére buffer
    allegro_exit();
    return 0;
}
END_OF_MAIN();
