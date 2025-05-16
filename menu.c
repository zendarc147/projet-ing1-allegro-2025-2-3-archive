#include "moteur.h"

//structure pour les boutons
typedef struct button{
    int x, y, width, height;
    char label[20];
} Button;

FONT *police_bouton;//ajouter une police windiws et utiliser l'appli
FONT *police2_bouton;

BITMAP *vaisseau1, *vaisseau2, *sith, *jedi,*stromtrooper,*wookiee;

BITMAP *fond_menu = NULL;


// États du menu
enum { MENU_MAIN, MENU_PLAYER_SELECT, MENU_CHOIX_PERSO };

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

    Button mainBtns[3], playerBtns[4];
    Button persoBtns[4];
    int attente_frames = 75;
    int delai_reapparition = attente_frames; // compteur de frames à attendre
    int state = MENU_MAIN;
    srand(time(NULL));

    fond_menu = load_bitmap("images/fond.bmp", NULL); // change le nom selon ton fichier
        if (!fond_menu) {
    allegro_message("Erreur chargement fond_menu.bmp");
    destroy_bitmap(vaisseau1);
    destroy_bitmap(vaisseau2);
    return -1;
    }

    police_bouton = load_font("images/police_bouton.pcx", NULL, NULL);
    if (!police_bouton) {
        allegro_message("Erreur chargement de la police des boutons !");
        allegro_exit();
        exit(EXIT_FAILURE);
}
    police2_bouton = load_font("images/police2_bouton.pcx", NULL, NULL);
    if (!police2_bouton) {
        allegro_message("Erreur chargement de la police des boutons !");
        allegro_exit();
        exit(EXIT_FAILURE);
}

    vaisseau1 = load_bitmap("images/vaisseau1.bmp", NULL);
    if (!vaisseau1) {
        allegro_message("Erreur chargement vaisseau1.bmp");
    }
    // Le sprite démarre à gauche de l'écran
    int vaisseau1_x = SCREEN_W; // hors écran à droite
    int vaisseau1_y = (SCREEN_H / 2 - vaisseau1->h / 2)+100;//centré verticalement

    vaisseau2 = load_bitmap("images/vaisseau2.bmp", NULL);
    if (!vaisseau2) {
        allegro_message("Erreur chargement vaisseau2.bmp");
    }
    sith = load_bitmap("images/sith_perso.bmp", NULL);
    if (!sith) {
        allegro_message("Erreur chargement sith.bmp");
    }
    jedi = load_bitmap("images/jedi_perso.bmp", NULL);
    if (!jedi) {
        allegro_message("Erreur chargement jedi.bmp");
    }
    wookiee = load_bitmap("images/chewbacca_perso.bmp", NULL);
    if (!wookiee) {
        allegro_message("Erreur chargement chewbacca.bmp");
    }
    stromtrooper = load_bitmap("images/strom_trooper_perso.bmp", NULL);
    if (!stromtrooper) {
        allegro_message("Erreur chargement strom_trooper.bmp");
    }


    // Création du buffer
    buffer = create_bitmap(SCREEN_W, SCREEN_H);
    if (!buffer) {
        allegro_message("Erreur création buffer");
        destroy_bitmap(vaisseau1);
        destroy_bitmap(vaisseau2);
        return -1;
    }

    show_mouse(NULL);

    // Définition des boutons du menu principal
    int w = 220, h = 60, e = 25;// w = largeur bouton, h = hauteur btn, e = espace bouton
    int total_height = h * 3 + e * 2;
    int y0 = (SCREEN_H - total_height) / 2;
    int x0 = (SCREEN_W - w) / 2;

    strcpy(mainBtns[0].label, "Jouer");      mainBtns[0].x = x0;               mainBtns[0].y = y0;
    mainBtns[0].width = w;                   mainBtns[0].height = h;
    strcpy(mainBtns[1].label, "Rejouer");    mainBtns[1].x = x0;               mainBtns[1].y = y0 + (h+e);
    mainBtns[1].width = w;                   mainBtns[1].height = h;
    strcpy(mainBtns[2].label, "Quitter");   mainBtns[2].x = x0;               mainBtns[2].y = y0 + 2*(h+e);
    mainBtns[2].width = w;                   mainBtns[2].height = h;

   // Définition des boutons de sélection joueurs (2 à 4 joueurs seulement)
    int pw = 200, ph = 50, pe = 15;
    int nb_joueurs = 3; // 2, 3, 4 joueurs => 3 boutons
    int total_player_height = ph * nb_joueurs + pe * (nb_joueurs - 1);
    int py0 = (SCREEN_H - total_player_height) / 2;
    int px0 = (SCREEN_W - pw) / 2;

    for (int i = 0; i < nb_joueurs; i++) {
        sprintf(playerBtns[i].label, "%d joueurs", i + 2); //i=0 -> "2 joueurs"
        playerBtns[i].x = px0;
        playerBtns[i].y = py0 + i * (ph + pe);
        playerBtns[i].width = pw;
        playerBtns[i].height = ph;
    }

    //Boutons pour l'affichage du Choix des perso
    int bw = 150, bh = 60, be = 60;//largeur, hauteur, espace
    int total_width = 4 * bw + 3 * be;
    int base_y = (SCREEN_H / 2);
    int base_x = (SCREEN_W - total_width) / 2;

    strcpy(persoBtns[0].label, "Stormtrooper");
    strcpy(persoBtns[1].label, "Sith");
    strcpy(persoBtns[2].label, "Wookiee");
    strcpy(persoBtns[3].label, "Jedi");

    for (int i = 0; i < 4; i++) {
        persoBtns[i].x = base_x + i * (bw + be);
        persoBtns[i].y = base_y;
        persoBtns[i].width = bw;
        persoBtns[i].height = bh;
    }

    float vaisseau2_x = SCREEN_W; // Hors de l'écran à droite
    float vaisseau2_y = 20; // Juste sous le 3e bouton
    const float vaisseau2_y_initial = 20;


    while (!key[KEY_ESC]) {
        // Affichage selon état
        if (state == MENU_MAIN) {
            stretch_blit(fond_menu, buffer, 0, 0, fond_menu->w, fond_menu->h, 0, 0, SCREEN_W, SCREEN_H);
            //clear_to_color(buffer, makecol(0, 0, 0)); // fond noir
            draw_sprite(buffer, vaisseau2, vaisseau2_x, vaisseau2_y);
            vaisseau2_x -= 1.5;//avance de 2 pixels par frame
            vaisseau2_y+=0.30;
            if (vaisseau2_x < -vaisseau2->w) {
                vaisseau2_x = SCREEN_W;
                vaisseau2_y = vaisseau2_y_initial;
            }
            textout_centre_ex(buffer, police_bouton, "ECE Arena", SCREEN_W / 2, y0 - 60, makecol(255,255,255), -1);
            for (int i=0; i<3; i++) dessiner_bouton(buffer, mainBtns[i]);
            //Afficher le vaisseau à gauche, position dynamike
        if (delai_reapparition > 0) {
            delai_reapparition--;
        } else {
            // mouvement
            vaisseau1_x -= 8;
            vaisseau1_y += 5;

            // s'il est sorti de l'écran
            if (vaisseau1_x < -vaisseau1->w || vaisseau1_y > SCREEN_H) {
                delai_reapparition = attente_frames; // déclenche le délai
            }
        }

        // Afficher le sprite uniquement si on n'est pas en attente
        if (delai_reapparition == 0) {
            draw_sprite(buffer, vaisseau1, vaisseau1_x, vaisseau1_y);
        }
        if (delai_reapparition == attente_frames) {
            vaisseau1_x = SCREEN_W;
            vaisseau1_y = (SCREEN_H / 2-180) + rand() % ((SCREEN_H / 2-50) - vaisseau1->h);
        }
        } else if (state == MENU_PLAYER_SELECT) {
            stretch_blit(fond_menu, buffer, 0, 0, fond_menu->w, fond_menu->h, 0, 0, SCREEN_W, SCREEN_H);
            textout_centre_ex(buffer, police2_bouton, "Nombre de Joueurs", SCREEN_W / 2, y0 - 60, makecol(255,255,255), -1);
            for (int i = 0; i < nb_joueurs; i++)dessiner_bouton(buffer, playerBtns[i]);
        } else if (state == MENU_CHOIX_PERSO) {
            stretch_blit(fond_menu, buffer, 0, 0, fond_menu->w, fond_menu->h, 0, 0, SCREEN_W, SCREEN_H);
            textout_centre_ex(buffer, police2_bouton, "Choix de votre Personnage", SCREEN_W / 2, 120, makecol(255, 255, 255), -1);
            for (int i = 0; i < 4; i++) {dessiner_bouton(buffer, persoBtns[i]);}
            //Ecriture des statistiques Perso 1
            textout_centre_ex(buffer, font, "Statistiques :",persoBtns[0].x-20 + persoBtns[0].width / 2,persoBtns[0].y+8 + persoBtns[0].height + 10,makecol(255, 255, 255), -1);
            textout_centre_ex(buffer, font, "PV :",persoBtns[0].x-20 + persoBtns[0].width / 2,persoBtns[0].y+30 + persoBtns[0].height + 10,makecol(255, 255, 255), -1);
            textout_centre_ex(buffer, font, "PM :",persoBtns[0].x-20 + persoBtns[0].width / 2,persoBtns[0].y+50 + persoBtns[0].height + 10,makecol(255, 255, 255), -1);
            textout_centre_ex(buffer, font, "Attaque :",persoBtns[0].x-20 + persoBtns[0].width / 2,persoBtns[0].y+70 + persoBtns[0].height + 10,makecol(255, 255, 255), -1);
            //Ecriture des statistiques Perso 2
            textout_centre_ex(buffer, font, "Statistiques :",persoBtns[1].x-20 + persoBtns[1].width / 2,persoBtns[1].y+8 + persoBtns[1].height + 10,makecol(255, 255, 255), -1);
            textout_centre_ex(buffer, font, "PV :",persoBtns[1].x-20 + persoBtns[1].width / 2,persoBtns[1].y+30 + persoBtns[1].height + 10,makecol(255, 255, 255), -1);
            textout_centre_ex(buffer, font, "PM :",persoBtns[1].x-20 + persoBtns[1].width / 2,persoBtns[1].y+50 + persoBtns[1].height + 10,makecol(255, 255, 255), -1);
            textout_centre_ex(buffer, font, "Attaque :",persoBtns[1].x-20 + persoBtns[1].width / 2,persoBtns[1].y+70 + persoBtns[1].height + 10,makecol(255, 255, 255), -1);
            //Ecriture des statistiques Perso 3
            textout_centre_ex(buffer, font, "Statistiques :",persoBtns[2].x-20 + persoBtns[2].width / 2,persoBtns[2].y+8 + persoBtns[2].height + 10,makecol(255, 255, 255), -1);
            textout_centre_ex(buffer, font, "PV :",persoBtns[2].x-20 + persoBtns[2].width / 2,persoBtns[2].y+30 + persoBtns[2].height + 10,makecol(255, 255, 255), -1);
            textout_centre_ex(buffer, font, "PM :",persoBtns[2].x-20 + persoBtns[2].width / 2,persoBtns[2].y+50 + persoBtns[2].height + 10,makecol(255, 255, 255), -1);
            textout_centre_ex(buffer, font, "Attaque :",persoBtns[2].x-20 + persoBtns[2].width / 2,persoBtns[2].y+70 + persoBtns[2].height + 10,makecol(255, 255, 255), -1);
            //Ecriture des statistiques Perso 4
            textout_centre_ex(buffer, font, "Statistiques :",persoBtns[3].x-20 + persoBtns[3].width / 2,persoBtns[3].y+8 + persoBtns[3].height + 10,makecol(255, 255, 255), -1);
            textout_centre_ex(buffer, font, "PV :",persoBtns[3].x-20 + persoBtns[3].width / 2,persoBtns[3].y+30 + persoBtns[3].height + 10,makecol(255, 255, 255), -1);
            textout_centre_ex(buffer, font, "PM :",persoBtns[3].x-20 + persoBtns[3].width / 2,persoBtns[3].y+50 + persoBtns[3].height + 10,makecol(255, 255, 255), -1);
            textout_centre_ex(buffer, font, "Attaque :",persoBtns[3].x-20 + persoBtns[3].width / 2,persoBtns[3].y+70 + persoBtns[3].height + 10,makecol(255, 255, 255), -1);
            //Affichage de l'image juste au-dessus des bouton
            draw_sprite(buffer,stromtrooper,persoBtns[0].x + persoBtns[0].width / 2 - sith->w / 2,persoBtns[0].y - sith->h - 10);
            draw_sprite(buffer,sith,persoBtns[1].x + persoBtns[1].width / 2 - sith->w / 2,persoBtns[1].y - sith->h - 10);
            draw_sprite(buffer,wookiee,persoBtns[2].x + persoBtns[2].width / 2 - sith->w / 2,persoBtns[2].y - sith->h - 10);
            draw_sprite(buffer,jedi,persoBtns[3].x + persoBtns[0].width / 2 - sith->w / 2,persoBtns[3].y - sith->h - 10);
        }
        //draw_sprite(buffer, mouse_sprite, mouse_x, mouse_y);
        show_mouse(buffer);  // dans la boucle
        blit(buffer, screen, 0,0, 0,0,SCREEN_W,SCREEN_H);

        if (mouse_b & 1) {
            int sel=-1;
            Button *list;
            int count;
            if (state == MENU_MAIN) {
                list = mainBtns;
                count = 3;
            } else if (state == MENU_PLAYER_SELECT) {
                list = playerBtns;
                count = nb_joueurs;
            } else if (state == MENU_CHOIX_PERSO) {
                list = persoBtns;
                count = 4;
            }
            for (int i=0; i<count; i++) {
                if (mouse_x>=list[i].x && mouse_x<list[i].x+list[i].width
                 && mouse_y>=list[i].y && mouse_y<list[i].y+list[i].height) { sel=i; break; }
            }
            if (sel != -1) {
                if (state == MENU_MAIN) {
                    if (sel == 0) state = MENU_PLAYER_SELECT;
                    else if (sel == 1) allegro_message("Rejouer");
                    else break;
                } else if (state == MENU_PLAYER_SELECT) {
                    nb_joueurs = sel + 2;
                    state = MENU_CHOIX_PERSO;
                } else if (state == MENU_CHOIX_PERSO) {
                    allegro_message("Perso %d sélectionné", sel + 1);

                }
            }
            while(mouse_b & 1) rest(10);
        }

        rest(20);
    }

    // Nettoyage
    destroy_bitmap(buffer);
    destroy_font(police_bouton);
    destroy_bitmap(sith);
    destroy_bitmap(jedi);
    destroy_bitmap(wookiee);
    destroy_bitmap(stromtrooper);
    destroy_bitmap(vaisseau2);
    destroy_bitmap(vaisseau1);
    destroy_bitmap(fond_menu);
    allegro_exit();
    return 0;
}