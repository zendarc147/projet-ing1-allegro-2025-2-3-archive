#include <allegro.h>
#include "moteur.h"

typedef struct button {
    int x, y, width, height;
    char label[20];
} Button;

// États du menu
enum { MENU_MAIN, MENU_PLAYER_SELECT };

// Dessine un bouton avec effet agrandissement au survol
void dessiner_bouton(BITMAP *bmp, Button b) {
    int survole = (mouse_x > b.x && mouse_x < b.x + b.width
                   && mouse_y > b.y && mouse_y < b.y + b.height);
    int offset = survole ? 5 : 0;
    rect(bmp,
         b.x - offset, b.y - offset,
         b.x + b.width + offset, b.y + b.height + offset,
         makecol(255, 255, 255));
    textout_centre_ex(bmp, font,
                      b.label,
                      b.x + b.width/2,
                      b.y + b.height/2 - 4,
                      makecol(255, 255, 255), -1);
}

// Fonction menu
int menu() {
    BITMAP *fond_ecran, *vaisseau;
    Button mainBtns[3], playerBtns[4];
    int state = MENU_MAIN;

    // Chargement des images
    fond_ecran = load_bitmap("images/hotplanet.bmp", NULL);
    if (!fond_ecran) {
        allegro_message("Erreur chargement hotplanet.bmp");
        return -1;
    }
    vaisseau = load_bitmap("images/vaisseau.bmp", NULL);
    if (!vaisseau) {
        allegro_message("Erreur chargement vaisseau.bmp");
        destroy_bitmap(fond_ecran);
        return -1;
    }

    // Création du buffer
    buffer = create_bitmap(640, 480);
    if (!buffer) {
        allegro_message("Erreur création buffer");
        destroy_bitmap(fond_ecran);
        destroy_bitmap(vaisseau);
        return -1;
    }

    show_mouse(NULL);

    // Définition des boutons du menu principal
    int w=150, h=40, e=20;
    int y0=150, x0=(640-w)/2;
    strcpy(mainBtns[0].label, "Jouer");      mainBtns[0].x = x0;               mainBtns[0].y = y0;
    mainBtns[0].width = w;                   mainBtns[0].height = h;
    strcpy(mainBtns[1].label, "Rejouer");    mainBtns[1].x = x0;               mainBtns[1].y = y0 + (h+e);
    mainBtns[1].width = w;                   mainBtns[1].height = h;
    strcpy(mainBtns[2].label, "Quitter");   mainBtns[2].x = x0;               mainBtns[2].y = y0 + 2*(h+e);
    mainBtns[2].width = w;                   mainBtns[2].height = h;

    // Définition des boutons de sélection joueurs
    int pw=200, ph=50, pe=15;
    int py0=180, px0=(640-pw)/2;
    for (int i = 0; i < 4; i++) {
        sprintf(playerBtns[i].label, "%d joueur%s", i+1, (i>0?"s":""));
        playerBtns[i].x = px0;
        playerBtns[i].y = py0 + i*(ph+pe);
        playerBtns[i].width = pw;
        playerBtns[i].height = ph;
    }

    while (!key[KEY_ESC]) {
        // Affichage selon état
        if (state == MENU_MAIN) {
            stretch_blit(fond_ecran, buffer, 0,0, fond_ecran->w, fond_ecran->h, 0,0,640,480);
            textout_centre_ex(buffer, font, "ECE Arena", 320, 50, makecol(255,255,255), -1);
            for (int i=0; i<3; i++) dessiner_bouton(buffer, mainBtns[i]);
        } else {
            blit(vaisseau, buffer, 0,0, 0,0, vaisseau->w, vaisseau->h);
            for (int i=0; i<4; i++) dessiner_bouton(buffer, playerBtns[i]);
        }
        draw_sprite(buffer, mouse_sprite, mouse_x, mouse_y);
        blit(buffer, screen, 0,0, 0,0,640,480);

        if (mouse_b & 1) {
            int sel=-1;
            Button *list = (state==MENU_MAIN? mainBtns : playerBtns);
            int count = (state==MENU_MAIN? 3 : 4);
            for (int i=0; i<count; i++) {
                if (mouse_x>=list[i].x && mouse_x<list[i].x+list[i].width
                 && mouse_y>=list[i].y && mouse_y<list[i].y+list[i].height) { sel=i; break; }
            }
            if (sel != -1) {
                if (state == MENU_MAIN) {
                    if (sel == 0) state = MENU_PLAYER_SELECT;
                    else if (sel == 1) allegro_message("Rejouer");
                    else break;
                } else {
                    allegro_message("%d joueur(s) sélectionné(s)", sel+1);
                    state = MENU_MAIN;
                }
            }
            while(mouse_b & 1) rest(10);
        }
        rest(20);
    }

    // Nettoyage
    destroy_bitmap(buffer);
    destroy_bitmap(vaisseau);
    destroy_bitmap(fond_ecran);
    allegro_exit();
    return 0;
}