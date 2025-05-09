#include <allegro.h>
#include "moteur.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Structure pour les boutons
typedef struct button {
    int x, y, width, height;
    char label[20];
} Button;

// Dessine un bouton avec effet d'agrandissement au survol
static void dessiner_bouton(BITMAP *bmp, Button b) {
    int survole = (mouse_x > b.x && mouse_x < b.x + b.width
                && mouse_y > b.y && mouse_y < b.y + b.height);
    int offset = survole ? 5 : 0;
    rect(bmp,
         b.x - offset, b.y - offset,
         b.x + b.width + offset, b.y + b.height + offset,
         makecol(255,255,255));
    textout_centre_ex(bmp, font,
                      b.label,
                      b.x + b.width/2,
                      b.y + b.height/2 - 4,
                      makecol(255,255,255), -1);
}

// Prototypes des fonctions appelées au clic
void move_action(int dir);
void spell_action(int idx);

// Boucle de jeu avec boutons de déplacement et de sorts
int jeu(t_perso *perso[4]) {
    srand(time(NULL));
    int fin = 0;

    // Boutons de déplacement
    Button moveBtns[4];
    const char *mLab[4] = {"Up","Down","Left","Right"};
    for (int i = 0; i < 4; i++) {
        strcpy(moveBtns[i].label, mLab[i]);
        moveBtns[i].width  = 60;
        moveBtns[i].height = 30;
        moveBtns[i].x      = 20;
        moveBtns[i].y      = 20 + i * 40;
    }

    // Boutons de sorts
    Button spellBtns[4];
    for (int i = 0; i < 4; i++) {
        sprintf(spellBtns[i].label, "S%d", i+1);
        spellBtns[i].width  = 60;
        spellBtns[i].height = 30;
        spellBtns[i].x      = 100;
        spellBtns[i].y      = 20 + i * 40;
    }

    show_mouse(NULL);

    while (!fin) {
        clear_bitmap(buffer);
        // Dessin des boutons
        for (int i = 0; i < 4; i++) dessiner_bouton(buffer, moveBtns[i]);
        for (int i = 0; i < 4; i++) dessiner_bouton(buffer, spellBtns[i]);

        draw_sprite(buffer, mouse_sprite, mouse_x, mouse_y);
        blit(buffer, screen, 0, 0, 0, 0, 640, 480);

        if (mouse_b & 1) {
            // Vérifier clic sur déplacement
            for (int i = 0; i < 4; i++) {
                if (mouse_x >= moveBtns[i].x && mouse_x < moveBtns[i].x + moveBtns[i].width
                 && mouse_y >= moveBtns[i].y && mouse_y < moveBtns[i].y + moveBtns[i].height) {
                    //move_action(i);
                    break;
                }
            }
            // Vérifier clic sur sort
            for (int i = 0; i < 4; i++) {
                if (mouse_x >= spellBtns[i].x && mouse_x < spellBtns[i].x + spellBtns[i].width
                 && mouse_y >= spellBtns[i].y && mouse_y < spellBtns[i].y + spellBtns[i].height) {
                    //spell_action(i);
                    break;
                }
            }
            // Attendre relâchement
            while (mouse_b & 1) rest(10);
        }
        rest(20);
    }

    return 0;
}