#include "moteur.h"

BITMAP *buffer = NULL;

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
    // Variables globales

    initialisation_allegro();
    //menu();
    int plateau[9] = {1, 1, 0, 1, 0, 1, 1, 1, 1};
    afficher_plateau(plateau,3 ,3);

    destroy_bitmap(buffer);//libére buffer
    allegro_exit();
    return 0;
}
END_OF_MAIN();
