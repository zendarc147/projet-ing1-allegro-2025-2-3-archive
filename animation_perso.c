#include "moteur.h"
#define NB_FRAMES 8
#define FRAME_DELAY 100

//animation chewbacca
int animation() {

    BITMAP *frames[NB_FRAMES];
    frames[0] = load_bitmap("images/images_animation/chewbacca1.bmp", NULL);
    frames[1] = load_bitmap("images/images_animation/chewbacca2.bmp", NULL);
    frames[2] = load_bitmap("images/images_animation/chewbacca3.bmp", NULL);
    frames[3] = load_bitmap("images/images_animation/chewbacca4.bmp", NULL);
    frames[4] = load_bitmap("images/images_animation/chewbacca5.bmp", NULL);
    frames[5] = load_bitmap("images/images_animation/chewbacca6.bmp", NULL);
    frames[6] = load_bitmap("images/images_animation/chewbacca7.bmp", NULL);
    frames[7] = load_bitmap("images/images_animation/chewbacca8.bmp", NULL);

    for (int i = 0; i < NB_FRAMES; i++) {
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
        current_frame = (current_frame + 1) % NB_FRAMES;
        rest(FRAME_DELAY);
    }

    for (int i = 0; i < NB_FRAMES; i++) {
        destroy_bitmap(frames[i]);
    }

    return 0;
}


