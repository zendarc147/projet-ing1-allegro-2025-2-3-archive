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

static void dessiner_bouton_sans_survol(BITMAP *bmp, Button b,int fill) {
    int survole = 0;
    int offset = survole ? 5 : 0;
    if(!fill)rect(bmp,b.x - offset, b.y - offset,b.x + b.width + offset, b.y + b.height + offset,makecol(255,255,255));
    else rectfill(bmp,b.x - offset, b.y - offset,b.x + b.width + offset, b.y + b.height + offset,makecol(0,204,255));
    textout_centre_ex(bmp, font,
                      b.label,
                      b.x + b.width/2,
                      b.y + b.height/2 - 4,
                      makecol(255,255,255), -1);
}




// Boucle de jeu avec boutons de déplacement et de sorts
int jeu(t_perso *perso) {
    srand(time(NULL));
    int fin = 0;
    int joueur = 0;
    int mode;
    int spell;
    char buf[32];
    // Constantes de taille & espacement
    const int BTN_W     = 60;
    const int BTN_H     = 30;
    const int SPACING   = 10;    // espace entre les boutons
    const int MARGIN_B  = 20;
    const int PADDING = 8;
    int line_h = text_height(font);
    int color  = makecol(255, 255, 255);// marge basse

    // Nombre total de boutons : 1 MOVE + 4 sorts
    const int N_BTNS    = 5;
    int totalWidth = N_BTNS * BTN_W + (N_BTNS - 1) * SPACING;
    int startX     = (SCREEN_W - totalWidth) / 2;
    int posY       = SCREEN_H - BTN_H - MARGIN_B;

    // --- Bouton unique MOVE ---
    Button moveBtn;
    strcpy(moveBtn.label, "MOVE");
    moveBtn.width  = BTN_W;
    moveBtn.height = BTN_H;
    moveBtn.x      = startX;
    moveBtn.y      = posY;

    Button hpBoxes[4];
    int boxW = SCREEN_W / 4;
    int boxH = 70;
    for (int i = 0; i < 4; i++) {
        hpBoxes[i].x      = i * boxW;
        hpBoxes[i].y      = 0;
        hpBoxes[i].width  = boxW;
        hpBoxes[i].height = boxH;
        sprintf(hpBoxes[i].label, "Joueur %d", i+1);
    }

    // --- Boutons de sorts (S1 à S4) ---
    Button spellBtns[4];
    for (int i = 0; i < 4; i++) {
        sprintf(spellBtns[i].label, "S%d", i+1);
        spellBtns[i].width  = BTN_W;
        spellBtns[i].height = BTN_H;
        // on déplace de (i+1) boutons pour se caler après MOVE
        spellBtns[i].x      = startX + (i + 1) * (BTN_W + SPACING);
        spellBtns[i].y      = posY;
    }

    Button fin_tour;
    strcpy(fin_tour.label, "fin du tour");
    fin_tour.width  = BTN_W*1.5;
    fin_tour.height = BTN_H;
    fin_tour.x      = startX + 5 * (BTN_W + SPACING);;
    fin_tour.y      = posY;

    Button info;
    strcpy(info.label, "");
    info.width  = BTN_W*1.5;
    info.height = BTN_H*4;
    info.x      = startX - 1.7*(BTN_W + SPACING); ;;
    info.y      = posY-BTN_H*3;


    show_mouse(NULL);

    while (!key[__allegro_KEY_ESC]) {

        clear_bitmap(buffer);
        // Dessin des boutons
        dessiner_bouton(buffer, moveBtn);
        for (int i = 0; i < 4; i++) dessiner_bouton(buffer, spellBtns[i]);
        dessiner_bouton(buffer, fin_tour);
        for (int i = 0; i < 4; i++) if(joueur ==i)dessiner_bouton_sans_survol(buffer,hpBoxes[i],1);else dessiner_bouton_sans_survol(buffer,hpBoxes[i],0);
        dessiner_bouton_sans_survol(buffer,info,0);


        // 1) Nom du perso
        buf[0] = '\0';
        switch(perso[joueur].classe) {
            case 0: strcpy(buf, "WOOK");       break;
            case 1: strcpy(buf, "ST"); break;
            case 2: strcpy(buf, "SITH");         break;
            case 3: strcpy(buf, "JEDI");         break;
            default: strcpy(buf, "??");          break;
        }


        textprintf_ex(buffer, font,
              info.x + PADDING,
              info.y + PADDING + line_h * 0,
              makecol(255,255,255),  // couleur texte
              -1,                    // fond transparent
              "PV : %s", buf);

        // 2) PV

        //sprintf(buf, "PV : %d", perso[joueur]->pv);
        textprintf_ex(buffer, font,
              info.x + PADDING,
              info.y + PADDING + line_h * 3,
              makecol(255,255,255),  // couleur texte
              -1,                    // fond transparent
              "PV : %d", perso[joueur].pv);

        // 3) PM
        buf[0] = '\0';
        //sprintf(buf, "PM : %d", perso[joueur]->pm);
        textprintf_ex(buffer, font,
              info.x + PADDING,
              info.y + PADDING + line_h * 6,
              makecol(255,255,255),  // couleur texte
              -1,                    // fond transparent
              "PM : %d", perso[joueur].pm);

        // 4) PA
        buf[0] = '\0';
        //sprintf(buf, "PA : %d", perso[joueur]->pa);
        textprintf_ex(buffer, font,
              info.x + PADDING,
              info.y + PADDING + line_h * 9,
              makecol(255,255,255),  // couleur texte
              -1,                    // fond transparent
              "PA : %d", perso[joueur].pa);

        draw_sprite(buffer, mouse_sprite, mouse_x, mouse_y);
        blit(buffer, screen, 0, 0, 0, 0, 640, 480);


        if (mouse_b & 1) {
            // Vérifier clic sur déplacement

            if (mouse_x >= moveBtn.x && mouse_x < moveBtn.x + moveBtn.width
            && mouse_y >= moveBtn.y && mouse_y < moveBtn.y + moveBtn.height) {
                if(mode =! 1){mode = 1;}else{mode = 0;}
                //move_action(i);
                break;
            }

            // Vérifier clic sur sort
            for (int i = 0; i < 4; i++) {
                if (mouse_x >= spellBtns[i].x && mouse_x < spellBtns[i].x + spellBtns[i].width
                 && mouse_y >= spellBtns[i].y && mouse_y < spellBtns[i].y + spellBtns[i].height) {
                    if(mode =! 2){mode = 2;spell = i;}else{mode = 0;spell = 0;}
                    //spell_action(i);
                    break;
                }
            }

            if (mouse_x >= fin_tour.x && mouse_x < fin_tour.x + fin_tour.width
            && mouse_y >= fin_tour.y && mouse_y < fin_tour.y + fin_tour.height) {
                if(joueur == 3){joueur = 0;}else{joueur += 1;}


            }

            // Attendre relâchement
            while (mouse_b & 1) rest(10);
        }
        rest(20);
    }

    return 0;
}