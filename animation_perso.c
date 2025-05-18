#include "moteur.h"

#define FRAME_DELAY 100 // délai entre les frames en ms
#define MAX_FRAMES 20

int jouer_animation(const char *nom, int nb_frames, const char *dossier,int x, int y) {//nom fichier, nb frames, chemin accès, x, y
                                                                                        // exemple appel de la focntion : jouer_animation("sith", 11, "images/images_animation", 320, 240);

    BITMAP *frames[MAX_FRAMES];//image dans tableau
    char chemin[256];// chaines carac piur fichier

    for (int i = 0; i < nb_frames; i++) {// boucle afficher image à la suite, image dans tableau
        snprintf(chemin, sizeof(chemin), "%s/%s%d.bmp", dossier, nom, i + 1);
        frames[i] = load_bitmap(chemin, NULL);
        if (!frames[i]) {
            allegro_message("Erreur chargement image : %s", chemin);
            for(int j = 0; j < i; j++) destroy_bitmap(frames[j]);
            return 1;
        }
    }

    int current_frame = 0;

    while (!key[KEY_ESC]) {
        clear_to_color(screen, makecol(0,0,0));
        draw_sprite(screen,frames[current_frame], x, y);//affiche frame
        current_frame = (current_frame+1)%nb_frames;//passe frame suivante
        rest(FRAME_DELAY);
    }

    for (int i = 0; i < nb_frames; i++){//libere frame
        destroy_bitmap(frames[i]);}
    return 0;
}
