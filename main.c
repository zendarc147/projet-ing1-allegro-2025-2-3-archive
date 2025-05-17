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
    buffer = create_bitmap(SCREEN_W, SCREEN_H);//création du buffer
}

int main() {

    initialisation_allegro();
    t_perso *perso = initialisation();
    t_plateau *plateau = initialisation_plateau();
    creer_plateau(plateau);
    afficher_plateau(*plateau);
    blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    //menu();
    //jeu(perso,plateau);
    while (!key[KEY_ESC]) {
        clear_to_color(buffer, makecol(0, 0, 0));
        afficher_plateau(*plateau);
        circlefill(buffer, mouse_x, mouse_y, 3, makecol(255, 0, 0));
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        case_survolee(*plateau);

    }
    destroy_bitmap(buffer);//libére buffer
    allegro_exit();
    return 0;
}
END_OF_MAIN();
