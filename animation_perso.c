#include "moteur.h"

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
}