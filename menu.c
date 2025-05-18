#include "moteur.h"

//structure pour les boutons
typedef struct button{
    int x, y, width, height;
    char label[20];
} Button;

FONT *police_bouton;//ajouter une police windiws et utiliser l'appli
FONT *police2_bouton;

BITMAP *vaisseau1,*vaisseau2,*sith,*jedi,*stromtrooper,*wookiee;

BITMAP *fond_menu = NULL;

//etats menu
enum { MENU_MAIN, MENU_PLAYER_SELECT, MENU_CHOIX_PERSO };

//dessine bouton avec agrandissement au survol
void dessiner_bouton(BITMAP *bmp, Button b) {
    srand(time(NULL));
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

void afficher_stats_personnage(BITMAP *buffer, FONT *font,Button persoBtn, int pv, int pm, char* att, int espace_att) {
    int x_centre = persoBtn.x + persoBtn.width/2;
    int y_base = persoBtn.y+persoBtn.height+ 10;

    char texte[64];

    textout_centre_ex(buffer, font, "Statistiques :", x_centre - 20, y_base + 8, makecol(255, 255, 255),-1);
    sprintf(texte, "PV : %d",pv);
    textout_centre_ex(buffer, font, texte, x_centre -43, y_base +30,makecol(255, 255, 255), -1);
    sprintf(texte, "PM : %d",pm);
    textout_centre_ex(buffer,font,texte, x_centre - 46, y_base + 50,makecol(255, 255, 255), -1);
    sprintf(texte, "%s",att);
    textout_centre_ex(buffer, font, texte, x_centre - espace_att, y_base + 70, makecol(255, 255, 255), -1);
}

int menu(t_plateau *plateau) {
    Button mainBtns[3], playerBtns[4];
    Button persoBtns[4];
    int attente_frames = 75;
    int delai_reapparition = attente_frames; // compteur de frames à attendre
    int state = MENU_MAIN;
    srand(time(NULL));
    t_perso joueurs[MAX_JOUEURS];
    int nb_joueurs = 0;
    int joueur_actuel = 0;


    fond_menu = load_bitmap("images/fond.bmp", NULL);
        if (!fond_menu) {
            allegro_message("Erreur chargement fond_menu.bmp");
            destroy_bitmap(vaisseau1);destroy_bitmap(vaisseau2);
            return -1;}

    police_bouton = load_font("images/police_bouton.pcx", NULL, NULL);
    if (!police_bouton) {
        allegro_message("Erreur chargement de la police bouton");
        allegro_exit();
        exit(EXIT_FAILURE);
}
    police2_bouton= load_font("images/police2_bouton.pcx", NULL, NULL);
    if (!police2_bouton) {
        allegro_message("Erreur chargement de la police bouton ");
        allegro_exit();
        exit(EXIT_FAILURE);
}

    vaisseau1 = load_bitmap("images/vaisseau1.bmp", NULL);
    if (!vaisseau1) {
        allegro_message("Erreur chargement vaisseau1.bmp");
    }

    int vaisseau1_x = SCREEN_W;// hors écran à droite
    int vaisseau1_y = (SCREEN_H/ 2 - vaisseau1->h / 2)+100;// centré verticalement

    vaisseau2 = load_bitmap("images/vaisseau2.bmp", NULL);
    if (!vaisseau2) {
        allegro_message("Erreur chargement vaisseau2.bmp");
    }
    sith =  load_bitmap("images/sith_perso.bmp", NULL);
    if (!sith) {
        allegro_message("Erreur chargement sith.bmp");
    }
    jedi = load_bitmap("images/images_animation/jedi1.bmp", NULL);
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

    buffer = create_bitmap(SCREEN_W, SCREEN_H);
    if (!buffer) {
        allegro_message("Erreur création buffer");
        destroy_bitmap(vaisseau1);
        destroy_bitmap(vaisseau2);
    return -1;
    }

    show_mouse(NULL);

    //définition des boutons du menu principal
    int w = 220, h = 60, e = 25;// w = largeur bouton, h = hauteur btn, e = espace bouton
    int total_height = h * 3 + e *2;
    int y0 = (SCREEN_H - total_height) / 2;
    int x0 = (SCREEN_W - w) / 2;

    strcpy(mainBtns[0].label, "Jouer");      mainBtns[0].x = x0;               mainBtns[0].y = y0;
    mainBtns[0].width = w;                   mainBtns[0].height = h;
    strcpy(mainBtns[1].label, "Rejouer");    mainBtns[1].x = x0;               mainBtns[1].y = y0 + (h+e);
    mainBtns[1].width = w;                   mainBtns[1].height = h;
    strcpy(mainBtns[2].label, "Quitter");   mainBtns[2].x = x0;               mainBtns[2].y = y0 + 2*(h+e);
    mainBtns[2].width = w;                   mainBtns[2].height = h;

    int pw = 200, ph = 50, pe = 15;
    nb_joueurs = 3; // 2, 3,4 joueurs = 3boutons
    int total_player_height = ph * nb_joueurs + pe * (nb_joueurs - 1);
    int py0 = (SCREEN_H - total_player_height) / 2;
    int px0 = (SCREEN_W - pw) / 2;

    for (int i = 0; i < nb_joueurs; i++) {
        sprintf(playerBtns[i].label, "%d joueurs", i + 2); //i=0 = 2joueurs
        playerBtns[i].x = px0;
        playerBtns[i].y = py0 + i * (ph + pe);
        playerBtns[i].width = pw;
        playerBtns[i].height = ph;}

    //boutons affichage du Choix des perso
    int bw = 150, bh = 60, be = 60;//largeur, hauteur, espace
    int total_width = 4 * bw + 3 * be;
    int base_y = (SCREEN_H / 2);
    int base_x = (SCREEN_W - total_width) / 2;

    strcpy(persoBtns[0].label, "StromTrooper");
    strcpy(persoBtns[1].label, "Sith");
    strcpy(persoBtns[2].label, "Wookiee");
    strcpy(persoBtns[3].label, "Jedi");

    for (int i = 0; i < 4;i++) {
        persoBtns[i].x = base_x + i *(bw + be);
        persoBtns[i].y = base_y;
        persoBtns[i].width = bw;
        persoBtns[i].height = bh;}

    float vaisseau2_x = SCREEN_W;
    float vaisseau2_y = 20;
    const float vaisseau2_y_initial = 20;

    SAMPLE *menu_music = load_sample("son/Theme_Song.wav");
    if (!menu_music) {
        allegro_message("Impossible de charger menu.wav");
        return 1;}

    play_sample(menu_music, 255, 128, 1000, TRUE);

    jeu_music = load_sample("son/jeu.wav");
    if (!jeu_music) {
        allegro_message("Impossible de charger jeu.wav");
        return 1;}
    for(int i = 0; i < 20; i++) {
        for(int j = 0; j < 10; j++) {
            plateau->plateau[i][j] = 0;
            if (rand() % 30 == 1) plateau->plateau[i][j] = -1;
        }
    }
    creer_plateau(plateau);

    while (!key[KEY_ESC]) {
        //affichage selon état
         if (state == MENU_MAIN) {
             stretch_blit(fond_menu, buffer, 0, 0, fond_menu->w, fond_menu->h, 0, 0, SCREEN_W, SCREEN_H);
             draw_sprite(buffer, vaisseau2, vaisseau2_x, vaisseau2_y);
             vaisseau2_x -= 1.5;//avance de 2 pixels par frame
             vaisseau2_y+=0.30;
             if (vaisseau2_x< -vaisseau2->w) {
                 vaisseau2_x = SCREEN_W;
                 vaisseau2_y = vaisseau2_y_initial;
             }
             textout_centre_ex(buffer, police_bouton, "ECE Arena", SCREEN_W / 2, y0 - 60, makecol(255,255,255), -1);
             for (int i=0; i<3; i++) dessiner_bouton(buffer, mainBtns[i]);
             //Afficher le vaisseau à gauche, position dynamike
             if (delai_reapparition > 0) {
                 delai_reapparition--;
             } else {
                 vaisseau1_x -= 8;
                 vaisseau1_y+= 5;
                 // s'il est sorti de l'écran
                 if (vaisseau1_x < -vaisseau1->w || vaisseau1_y > SCREEN_H) {
                     delai_reapparition = attente_frames;
                 }
             }

             if (delai_reapparition == 0) {
                 draw_sprite(buffer, vaisseau1, vaisseau1_x,vaisseau1_y);
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
             int aggrad = 5;//aggrandissement
             stretch_blit(fond_menu, buffer, 0, 0, fond_menu->w, fond_menu->h, 0, 0, SCREEN_W, SCREEN_H);
             textout_centre_ex(buffer, police2_bouton, "Choix de votre Personnage",SCREEN_W / 2, 120,makecol(255, 255,255),-1);
             char msg[64];
             sprintf(msg, "Joueur %d, choisissez votre personnage", joueur_actuel + 1);
             textout_centre_ex(buffer,police2_bouton,msg, SCREEN_W / 2, 180, makecol(255, 255, 255), -1);
             for (int i = 0; i < 4; i++) {
                 dessiner_bouton(buffer, persoBtns[i]);
                 switch(i) {
                     case 0 :
                         afficher_stats_personnage(buffer,font,persoBtns[i],70,10, "Fusil Blaster", 25);break;
                     case 1 :
                         afficher_stats_personnage(buffer,font,persoBtns[i],100,20, "Sabre Laser / Force", 1);break;
                     case 2 :
                         afficher_stats_personnage(buffer,font,persoBtns[i],120,15, "Arbalète", 42);break;
                     case 3 :
                         afficher_stats_personnage(buffer,font,persoBtns[i],80,25, "Sabre Laser / Force", 1);break;

                 }
             }

            //afficher image au dessu des boutons
            draw_sprite(buffer,stromtrooper,persoBtns[0].x + persoBtns[0].width / 2 - sith->w / 2,persoBtns[0].y - sith->h+10);
            draw_sprite(buffer,sith,persoBtns[1].x + persoBtns[1].width/ 2 - sith->w /2,persoBtns[1].y- sith->h - 10);
            draw_sprite(buffer,wookiee,persoBtns[2].x + persoBtns[2].width / 2 - sith->w / 2,persoBtns[2].y - sith->h -10);
            stretch_sprite(buffer,jedi,persoBtns[3].x + persoBtns[3].width / 2 - (jedi->w * aggrad) / 2,persoBtns[3].y - (jedi->h * aggrad) - 10,jedi->w * aggrad,jedi->h * aggrad);
            //draw_sprite(buffer,jedi,persoBtns[3].x + persoBtns[0].width / 2 - sith->w / 2,persoBtns[3].y - sith->h - 10);
        }
        show_mouse(buffer);
        blit(buffer,screen, 0, 0, 0 , 0,SCREEN_W,SCREEN_H);
    plateau->plateau[5][5] = -1;

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
                if  (state == MENU_MAIN) {
                    if (sel == 0) state = MENU_PLAYER_SELECT;
                    else if (sel == 1) allegro_message("");
                    else break;
                } else if (state == MENU_PLAYER_SELECT) {
                    nb_joueurs = sel + 2;
                    state = MENU_CHOIX_PERSO;
                } else if (state == MENU_CHOIX_PERSO) {
                    //initialisation du joueur actuel
                    joueurs[joueur_actuel].joueur = joueur_actuel;
                    joueurs[joueur_actuel].classe = sel ; // 0: Storm, 1: Sith, 2: Wookiee, 3: Jedi
                    joueurs[joueur_actuel].ordre_mort=-1;

                    //initialisation selon la classe choisie
                    switch (joueurs[joueur_actuel].classe) {
                        case 0: //Stromtrooper
                            joueurs[joueur_actuel].pv = 70;
                            joueurs[joueur_actuel].pa= 2;
                            joueurs[joueur_actuel].pm = 10;
                        break;
                        case 1: //Sith
                            joueurs[joueur_actuel].pv = 100;
                            joueurs[joueur_actuel].pa = 2;
                            joueurs[joueur_actuel].pm = 20;
                            break;
                        case 2 : //Wookiee
                            joueurs[joueur_actuel].pv = 120;
                            joueurs[joueur_actuel].pa = 2;
                            joueurs[joueur_actuel].pm= 15;
                            break;
                        case 3: //Jedi
                            joueurs[joueur_actuel].pv = 80;
                            joueurs[joueur_actuel].pa = 2;
                            joueurs[joueur_actuel].pm = 25;
                            break;
                    }

                    for (int j = 0; j < 4; j++) {

                        joueurs[joueur_actuel].attaque[j] = *attaques[joueurs[joueur_actuel].classe][j];

                        //joueurs[joueur_actuel].classe vaut entre 1 et 4 car 4 classes, sith, jedi...
                        ////classe -1 car tableau commence par 0
                    }
                    int x, y;
                    int occupied;

                    do {
                        x = rand() % 12;
                        rest(30);
                        y = rand() % 12;

                        // Vérifier si un autre joueur occupe déjà cette case
                        occupied = 0;
                        for (int i = 0; i < nb_joueurs; i++) {
                            if (i != joueur_actuel
                             && joueurs[i].x == x
                             && joueurs[i].y == y)
                            {
                                occupied = 1;
                                break;
                            }
                        }
                    } while (occupied);

                    // Maintenant la case (x,y) est libre
                    joueurs[joueur_actuel].x = x;
                    joueurs[joueur_actuel].y = y;

                    joueur_actuel++;
                    if (joueur_actuel >= nb_joueurs) {
                        /*
                        for(int vol=255;vol >=0;vol -=5) {
                            adjust_sample(menu_music,vol,128,1000,TRUE);
                            rest(30);
                        }
                        for(int vol1=0;vol1<=255;vol1 +=5) {
                            play_sample(jeu_music,0,128,1000,TRUE);
                            adjust_sample(jeu_music,vol1,128,1000,TRUE);
                            rest(30);
                        }
                        */
                        stop_sample(menu_music);
                        play_sample(jeu_music,255,128,1000,TRUE);
                        jeu(joueurs,plateau,nb_joueurs);
                    }
                }
            }
            while(mouse_b & 1) rest(10);
        }
        rest(20);
    }
    destroy_bitmap(buffer);
    destroy_font(police_bouton);
    destroy_bitmap(sith);
    destroy_bitmap(jedi);
    destroy_bitmap(wookiee);
    destroy_bitmap(stromtrooper);
    destroy_bitmap(vaisseau2);
    destroy_bitmap(vaisseau1);
    destroy_bitmap(fond_menu);
    return 0;
}

