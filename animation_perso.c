/*#include "moteur.h"

#define FRAME_DELAY 100//tesmpe netre les frames en milisec

//animation chewbacca
int animation_chewbacca() {
    int nb_frames = 8;

    BITMAP *frames[8];
    frames[0] = load_bitmap("images/images_animation/chewbacca1.bmp", NULL);
    frames[1] = load_bitmap("images/images_animation/chewbacca2.bmp", NULL);
    frames[2] = load_bitmap("images/images_animation/chewbacca3.bmp", NULL);
    frames[3] = load_bitmap("images/images_animation/chewbacca4.bmp", NULL);
    frames[4] = load_bitmap("images/images_animation/chewbacca5.bmp", NULL);
    frames[5] = load_bitmap("images/images_animation/chewbacca6.bmp", NULL);
    frames[6] = load_bitmap("images/images_animation/chewbacca7.bmp", NULL);
    frames[7] = load_bitmap("images/images_animation/chewbacca8.bmp", NULL);

    for (int i = 0; i < nb_frames; i++) {
        if (!frames[i]) {
            allegro_message("Erreur lors du chargement de chewbacca%d.bmp", i+1);
            return 1;
        }
    }
    int current_frame = 0;
    int x = 320, y = 240;

    while (!key[KEY_ESC]) {
        clear_to_color(screen, makecol(0, 0, 0));
        draw_sprite(screen, frames[current_frame], x, y);
        current_frame = (current_frame + 1) % nb_frames;
        rest(FRAME_DELAY);
    }

    for (int i = 0; i < nb_frames; i++) {
        destroy_bitmap(frames[i]);
    }

    return 0;
}

int animation_sith() {

    int nb_frames = 11;

    BITMAP *frames[11];
    frames[0] = load_bitmap("images/images_animation/sith1.bmp", NULL);
    frames[1] = load_bitmap("images/images_animation/sith2.bmp", NULL);
    frames[2] = load_bitmap("images/images_animation/sith3.bmp", NULL);
    frames[3] = load_bitmap("images/images_animation/sith4.bmp", NULL);
    frames[4] = load_bitmap("images/images_animation/sith5.bmp", NULL);
    frames[5] = load_bitmap("images/images_animation/sith6.bmp", NULL);
    frames[6] = load_bitmap("images/images_animation/sith7.bmp", NULL);
    frames[7] = load_bitmap("images/images_animation/sith8.bmp", NULL);
    frames[8] = load_bitmap("images/images_animation/sith9.bmp", NULL);
    frames[9] = load_bitmap("images/images_animation/sith10.bmp", NULL);
    frames[10] = load_bitmap("images/images_animation/sith11.bmp", NULL);


    for (int i = 0; i < nb_frames; i++) {
        if (!frames[i]) {
            allegro_message("Erreur lors du chargement de chewbacca%d.bmp", i+1);
            return 1;
        }
    }
    int current_frame = 0;
    int x = 320, y = 240;

    while (!key[KEY_ESC]) {
        clear_to_color(screen, makecol(0, 0, 0));
        draw_sprite(screen, frames[current_frame], x, y);
        current_frame = (current_frame + 1) % nb_frames;
        rest(FRAME_DELAY);
    }

    for (int i = 0; i < nb_frames; i++) {
        destroy_bitmap(frames[i]);
    }

    return 0;
}*/

#include "moteur.h"

#define FRAME_DELAY 100 // délai entre les frames en ms
#define MAX_FRAMES 20   // limite max de frames (modifiable)

int jouer_animation(const char *nom, int nb_frames, const char *dossier,int x, int y) {//nom fichier, nb frames, chemin accès, x, y

    BITMAP *frames[MAX_FRAMES];//image dans tableau
    char chemin[256];// chaines carac piur fichier

    for (int i = 0; i < nb_frames; i++) {// boucle afficher image à la suite, image dans tableau
        snprintf(chemin, sizeof(chemin), "%s/%s%d.bmp", dossier, nom, i + 1);
        frames[i] = load_bitmap(chemin, NULL);
        if (!frames[i]) {
            allegro_message("Erreur chargement image : %s", chemin);
            // Libérer les bitmaps déjà chargés
            for (int j = 0; j < i; j++) destroy_bitmap(frames[j]);
            return 1;
        }
    }

    int current_frame = 0;

    while (!key[KEY_ESC]) {
        clear_to_color(screen, makecol(0, 0, 0));
        draw_sprite(screen, frames[current_frame], x, y);//affiche frame
        current_frame = (current_frame + 1) % nb_frames;//passe frame suivante
        rest(FRAME_DELAY);
    }

    for (int i = 0; i < nb_frames; i++) {//libere frame
        destroy_bitmap(frames[i]);
    }

    return 0;
}// exemple appel de la focntion : jouer_animation("sith", 11, "images/images_animation", 320, 240);
