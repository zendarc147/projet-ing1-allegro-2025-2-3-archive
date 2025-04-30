#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro.h>
#include "moteur.h"

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

int main() {

    destroy_bitmap(buffer);//libére buffer
    destroy_bitmap(fond_ecran);
    allegro_exit();
    return 0;
}
END_OF_MAIN();
