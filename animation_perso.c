#include "moteur.h"

void animations(t_plateau plateau,int perso_classe,int case_x,int case_y,int start){
    // Variables statiques conservent l’état entre les appels
    static int  active    = 0;
    static int  startTime = 0;
    static char animName[32];
    static int  nbFrames  = 0;
    const int   frameDur  = 100;  // ms par frame

    // Si on demande de démarrer l’anim
    if (start) {
        active = 1;
        startTime = clock() * 1000 / CLOCKS_PER_SEC;
        switch (perso_classe) {
            case 0: strcpy(animName, "stromtrooper"); nbFrames = 4;  break;
            case 1: strcpy(animName, "sith");         nbFrames = 11; break;
            case 2: strcpy(animName, "chewbacca");    nbFrames = 8;  break;
            case 3: strcpy(animName, "jedi");         nbFrames = 13; break;
            default: active = 0; return;
        }
    }

    // Si rien n’est actif, on ne fait rien
    if (!active) return;

    // Calcul du numéro de frame courant
    int now     = clock() * 1000 / CLOCKS_PER_SEC;
    int elapsed = now - startTime;
    int idx     = elapsed / frameDur;

    // Si on a dépassé le nombre de frames, on arrête l’anim
    if (idx >= nbFrames) {
        active = 0;
        return;
    }

    // Conversion de la case en pixels (centre-bas)
    int px, py;
    coor_case(plateau, case_x, case_y, &px, &py);

    // Chargement de la bitmap de la frame
    char path[256];
    snprintf(path, sizeof(path),
             "images/images_animation/%s%d.bmp",
             animName, idx);
    BITMAP *frame = load_bitmap(path, NULL);
    if (!frame) return;

    // Calcul du coin haut-gauche pour centrer et aligner le bas
    int w = frame->w  * plateau.taille;
    int h = frame->h  * plateau.taille;
    int dx = px - w/2;
    int dy = py - h;

    // Dessiner dans le buffer (avant blit)
    stretch_sprite(buffer, frame, dx, dy, w, h);
    destroy_bitmap(frame);
}
