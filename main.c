#include "moteur.h"

BITMAP *buffer = NULL;
BITMAP *mask = NULL;
SAMPLE *jeu_music = NULL;
volatile int quitter = 0;

static void fermer_fenetre(void) {
    quitter = 1;
}

t_perso *perso;


void initialisation_allegro() {
    srand(time(NULL));
    if (allegro_init() != 0) {
        fprintf(stderr, "Impossible d'initialiser Allegro : %s\n", allegro_error);
        exit(EXIT_FAILURE);
    }
    install_keyboard();
    install_mouse();
    set_color_depth(desktop_color_depth());
    if(set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1000, 800, 0, 0) != 0) {
        allegro_message("probleme mode graphique");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    buffer = create_bitmap(SCREEN_W, SCREEN_H);//création du buffer
    mask = create_bitmap(SCREEN_W, SCREEN_H);
    if (!buffer || !mask) {
        allegro_message("Impossible de créer les buffers graphiques");
        exit(EXIT_FAILURE);
    }
    set_window_title("ECE Arena");
    set_close_button_callback(fermer_fenetre);
    if (install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL) != 0) {
        fprintf(stderr, "Erreur audio : %s\n", allegro_error);
    }
}

int main() {

    initialisation_allegro();
    t_plateau *plateau = initialisation_plateau();
    if (!plateau) {
        allegro_exit();
        return EXIT_FAILURE;
    }
    int resultat = menu(plateau);
    show_mouse(NULL);
    if (jeu_music) destroy_sample(jeu_music);
    if (plateau->bitmap) destroy_bitmap(plateau->bitmap);
    free(plateau);
    destroy_bitmap(mask);
    destroy_bitmap(buffer);
    allegro_exit();
    return resultat == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
END_OF_MAIN();
