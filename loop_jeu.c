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
int temps_depart = 0;
int temps_total = 15000; // en millisecondes
int timer_actif = 1 ;

void dessiner_timer_rect(BITMAP *bmp, Button b,int *joueur,int reset,int nb_joueur) {
    if (!timer_actif) return;

    int temps_ecoule = clock() * 1000 / CLOCKS_PER_SEC - temps_depart;
    int temps_restant = temps_total - temps_ecoule;
    if (temps_restant < 0) temps_restant = 0;
    float ratio = (float)temps_ecoule / (float)temps_total;

    int red   = (int)(ratio * 255.0f);
    int vert  = 255 - red;

    int color = makecol(red, vert, 0);

    // --- Ajout pour boucler le timer ---
    if (temps_restant == 0 ||reset) {
        // on redémarre le timer
        temps_depart = clock() * 1000 / CLOCKS_PER_SEC;
        temps_restant = temps_total;
        if(!reset)if(*joueur == nb_joueur-1){*joueur = 0;}else{*joueur += 1;}


    }
    // -------------------------------------

    float proportion = (float)temps_restant / temps_total;
    int largeur_timer = (int)(b.width * proportion);

    rect(bmp, b.x, b.y, b.x + b.width, b.y + b.height, makecol(255, 255, 255));

    rectfill(bmp,
             b.x,
             b.y,
             b.x + largeur_timer,
             b.y + b.height,
             color);

    textout_centre_ex(bmp, font,
                      b.label,
                      b.x + b.width / 2,
                      b.y + b.height / 2 - 4,
                      makecol(255, 255, 255), -1);


}
// Boucle de jeu avec boutons de déplacement et de sorts
int jeu(t_perso *perso,t_plateau *plateau,int nb_joueur) {

    srand(time(NULL));
    int fin = 0;
    int joueur = 0;
    int tour = 99;
    int mode = 0;
    int spell;
    char buf[32];
    // Constantes de taille & espacement
    const int BTN_W     = 60*1.7;
    const int BTN_H     = 30*1.7;
    const int SPACING   = 10*1.7;    // espace entre les boutons
    const int MARGIN_B  = 20*1.7;
    const int PADDING = 8*1.7;
    int line_h = text_height(font);
    int color  = makecol(255, 255, 255);// marge basse

    // Nombre total de boutons : 1 MOVE + 4 sorts + 1 attaque
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
    for (int j = 0; j < 4; j++) {
        sprintf(spellBtns[j].label, "S%d", j+1);
        spellBtns[j].width  = BTN_W;
        spellBtns[j].height = BTN_H;
        // on déplace de (i+1) boutons pour se caler après MOVE
        spellBtns[j].x      = startX + (j + 1) * (BTN_W + SPACING);
        spellBtns[j].y      = posY;
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
    info.height = BTN_H*3;
    info.x      = startX - 1.7*(BTN_W + SPACING); ;;
    info.y      = posY-BTN_H*4;

    Button poin;
    strcpy(poin.label, "coup");
    poin.width  = BTN_W;
    poin.height = BTN_H;
    poin.x      = startX - 1*(BTN_W + SPACING); ;;
    poin.y      = posY;

    Button timer;
    strcpy(timer.label, "timer");
    timer.width  = SCREEN_W - BTN_W*2;
    timer.height = 25;
    timer.x      = startX - (BTN_W + SPACING) ; ;;
    timer.y      = posY - 40;


    dessiner_timer_rect(buffer, timer,&joueur,1,nb_joueur);

    show_mouse(NULL);
    SAMPLE *jeu_music = load_sample("son/jeu.wav");
    if (!jeu_music) {
        allegro_message("Impossible de charger jeu.wav");
        return 1;}
    

    while (!fin) {
        int done = 0;
        for (int i = 0; i < nb_joueur; i++) {
            if(perso[i].pv<=0){done += 1;}
        }
        if (done == nb_joueur-1) fin = 1;
        clear_bitmap(buffer);

        // Dessin des boutons
        afficher_plateau(*plateau, perso, nb_joueur);
        dessiner_bouton(buffer, moveBtn);
        for (int i = 0; i < 4; i++) dessiner_bouton(buffer, spellBtns[i]);
        dessiner_bouton(buffer, fin_tour);
        for (int i = 0; i < nb_joueur; i++) if(joueur ==i)dessiner_bouton_sans_survol(buffer,hpBoxes[i],1);else dessiner_bouton_sans_survol(buffer,hpBoxes[i],0);
        dessiner_bouton_sans_survol(buffer,info,0);
        dessiner_bouton(buffer,poin);
        //for (int i = 0; i < nb_joueur; i++) afficher_perso(*plateau,perso[i].classe,perso[i].y,perso[i].x);




        if(tour != joueur) {
            tour = joueur;
            perso[joueur].pa += 2;
            perso[joueur].pm = 3;
        }

        // 1) Nom du perso
        buf[0] = '\0';
        switch(perso[joueur].classe) {
            case 0: strcpy(buf, "ST");       break;
            case 1: strcpy(buf, "SITH");          break;
            case 2: strcpy(buf, "WOOKI");         break;
            case 3: strcpy(buf, "JEDI");         break;
            default: strcpy(buf, "??");          break;
        }

        //Classe
        textprintf_ex(buffer, font,
              info.x + PADDING,
              info.y + PADDING + line_h * 0,
              makecol(255,255,255),  // couleur texte
              -1,                    // fond transparent
              "Classe : %s", buf);

        // 2) PV

        //sprintf(buf, "PV : %d", perso[joueur]->pv);
        textprintf_ex(buffer, font,
              info.x + PADDING,
              info.y + PADDING + line_h * 3,
              makecol(255,255,255),  // couleur texte
              -1,                    // fond transparent
              "PV : %d", perso[joueur].pv);

        // 3) PM

        //sprintf(buf, "PM : %d", perso[joueur]->pm);
        textprintf_ex(buffer, font,
              info.x + PADDING,
              info.y + PADDING + line_h * 6,
              makecol(255,255,255),  // couleur texte
              -1,                    // fond transparent
              "PM : %d", perso[joueur].pm);

        // 4) PA

        //sprintf(buf, "PA : %d", perso[joueur]->pa);
        textprintf_ex(buffer, font,
              info.x + PADDING,
              info.y + PADDING + line_h * 9,
              makecol(255,255,255),  // couleur texte
              -1,                    // fond transparent
              "PA : %d", perso[joueur].pa);
        //Timer
        dessiner_timer_rect(buffer, timer,&joueur,0,nb_joueur); // dessin sur le buffer


        for (int i = 0; i < nb_joueur; i++) {
            switch(perso[i].classe) {
                case 0: strcpy(buf, "ST");       break;
                case 1: strcpy(buf, "SITH");          break;
                case 2: strcpy(buf, "WOOKI");         break;
                case 3: strcpy(buf, "JEDI");         break;
                default: strcpy(buf, "??");          break;
            }
            //Classe en haut
            textprintf_ex(buffer, font,
                  hpBoxes[i].x + PADDING,
                  hpBoxes[i].y + PADDING + line_h * 5,
                  makecol(255,255,255),  // couleur texte
                  -1,                    // fond transparent
                  "Classe : %s",buf);

            // 2) PV en haut
            //sprintf(buf, "PV : %d", perso[joueur]->pv);
            if(perso[i].pv > 0) {
                textprintf_ex(buffer, font,
                      hpBoxes[i].x + PADDING,
                      hpBoxes[i].y + PADDING + line_h * 3,
                      makecol(255,255,255),  // couleur texte
                      -1,                    // fond transparent
                      "PV : %d", perso[i].pv);
            } else {
                textprintf_ex(buffer, font,
                      hpBoxes[i].x + PADDING,
                      hpBoxes[i].y + PADDING + line_h * 3,
                      makecol(255,255,255),  // couleur texte
                      -1,                    // fond transparent
                      "mort");
            }
        }
        for(int i = 0; i < 4; i++) {
            textprintf_ex(buffer, font,
                spellBtns[i].x + PADDING/2,
                spellBtns[i].y + PADDING*3,
                makecol(255,255,255),
                -1,
                "PA:%d",perso[joueur].attaque[i].cout);

            textprintf_ex(buffer, font,
                spellBtns[i].x + PADDING*4,
                spellBtns[i].y + PADDING*3,
                makecol(255,255,255),
                -1,
                "dgt:%d",(perso[joueur].attaque[i].degats_max + perso[joueur].attaque[i].degats_min)/2);
        }
        draw_sprite(buffer, mouse_sprite, mouse_x, mouse_y);

        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        //blit(plateau->mask, screen, 0, 0, 0, 0, 640, 480);

        if(perso[joueur].pv <= 0){if(joueur == nb_joueur-1){joueur = 0;}else{joueur += 1;}}
        if (mouse_b & 1) {
            // Vérifier clic sur déplacement
            int color = getpixel(mask, mouse_x, mouse_y);
            int row = getr(color);
            int col = getg(color);

            if (mouse_x >= moveBtn.x && mouse_x < moveBtn.x + moveBtn.width
            && mouse_y >= moveBtn.y && mouse_y < moveBtn.y + moveBtn.height) {
                if(mode != 1){mode = 1;}else{mode = 0;}
            }

            // Vérifier clic sur sort
            for (int i = 0; i < 4; i++) {
                if (mouse_x >= spellBtns[i].x && mouse_x < spellBtns[i].x + spellBtns[i].width
                 && mouse_y >= spellBtns[i].y && mouse_y < spellBtns[i].y + spellBtns[i].height) {
                    if(mode != 2){mode = 2;spell = i;}else{mode = 0;spell = 0;}

                    //spell_action(i);
                    break;
                }
            }
            //Vérifier clic sur fin du tour
            if (mouse_x >= fin_tour.x && mouse_x < fin_tour.x + fin_tour.width
            && mouse_y >= fin_tour.y && mouse_y < fin_tour.y + fin_tour.height) {
                if(joueur == nb_joueur-1){joueur = 0;}else{joueur += 1;}
                dessiner_timer_rect(buffer, timer,&joueur,1,nb_joueur);

            }

            if (mouse_x >= poin.x && mouse_x < poin.x + poin.width
            && mouse_y >= poin.y && mouse_y < poin.y + poin.height) {
                if(mode != 3){mode = 3;}else{mode = 0;}


            }

            // Attendre relâchement
            while (mouse_b & 1) rest(10);
            gerer_mode(mode,&perso[joueur],spell,plateau->plateau);
            //printPlateau(plateau->plateau,20,20);
            creer_plateau(plateau);

            if (mode == 1 && plateau->plateau[row][col]<200 && plateau->plateau[row][col] >= 100 && row != 255) {
                int done = 0;
                while (mouse_b & 1) rest(10);
                for (int i = 0; i < nb_joueur; i++) {
                    if(perso[i].x!=row || perso[i].y!=col ) {done += 1;}
                    if(done == nb_joueur) {
                        perso[joueur].x = row;
                        perso[joueur].y = col;
                        mode = 0;
                        gerer_mode(mode,&perso[joueur],spell,plateau->plateau);
                        creer_plateau(plateau);
                    }
                }
            }
            if (mode == 2 && plateau->plateau[row][col]<300 && plateau->plateau[row][col] >= 200 && row != 255 ) {
                while (mouse_b & 1) rest(10);
                gerer_attaques(&perso[joueur],perso,spell,row,col);
                mode = 0;
                gerer_mode(mode,&perso[joueur],spell,plateau->plateau);
                creer_plateau(plateau);
            }
            if (mode == 3 && plateau->plateau[row][col]<300 && plateau->plateau[row][col] >= 200 && row != 255 ) {
                while (mouse_b & 1) rest(10);
                gerer_attaques(&perso[joueur],perso,5,row,col);
                mode = 0;
                gerer_mode(mode,&perso[joueur],spell,plateau->plateau);
                creer_plateau(plateau);
            }
        }

    }

    destroy_sample(jeu_music);

    return 0;
}

