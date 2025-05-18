#include "moteur.h"

BITMAP *win;

void ecran_fin(t_perso *perso, int nb_joueurs,t_plateau *plateau) {
    win = load_bitmap("images/win.bmp", NULL);
    if (!win) {
        allegro_message("Erreur chargement win.bmp");
        return;
    }
    stop_sample(jeu_music);

    clear_to_color(screen, makecol(0,0,0));

    int x = (SCREEN_W - win->w)/2;
    int y  =(SCREEN_H - win->h-300)/2;

    draw_sprite(screen, win,x,y);

    int gagnant = -1;
    int perdus[4];//4 = Max_joueurs
    int ordre = 0;

    for(int i =0;i< 4;i++)perdus[i]=-1;

    for(int i=0; i < nb_joueurs;i++) {//Trouver gagnant
        if(perso[i].pv >0) {
            gagnant = i;
        }
    }

    //Ordre des joueurs éliminés
    for(int place=nb_joueurs -1;place >=1;place--) {
        for(int i =0; i <nb_joueurs;i++) {
            if(perso[i].pv <=0 && perdus[i]==-1) {
                perdus[i]=place;
                break;
            }
        }
    }

    char texte[64];
    sprintf(texte, "Joueur %d, a gagné ! ", perso[gagnant].joueur +1);//+1 pour affichage normal
    textout_centre_ex(screen, font, texte, SCREEN_W/2,SCREEN_H/2, makecol(0,255,0),-1);

    //affichage des autres 2e 3e etc...
    int ligne = 1;
    for(int ordre =1; ordre < nb_joueurs;ordre++) {
        for(int i =0; i <nb_joueurs;i++) {
            if(perso[i].ordre_mort==ordre) {
                sprintf(texte, "%d : Joueur %d", ordre +1, perso[i].joueur+1);
                textout_centre_ex(screen, font, texte, SCREEN_W/2, SCREEN_H/2+ligne * 30, makecol(255,255,255),-1);
                ligne++;
            }
        }
    }

    //bouton quitter
    char * msg = "Quitter";
    char * msg2 = "rejouer";
    int text_w = text_length(font,msg);//longueur texte
    int text_h = text_height(font);//largeur police

    int text_x = SCREEN_W/2;
    int text_y = SCREEN_H - 90;

    textout_centre_ex(screen,font,msg, text_x, text_y, makecol(25,255,255),-1);
    textout_centre_ex(screen,font,msg2, text_x, text_y-40, makecol(25,255,255),-1);
    show_mouse(screen);
    blit(screen, screen, 0,0,0,0, SCREEN_W,SCREEN_H);

    while(!key[KEY_ESC]) {
        if(mouse_b & 1) {
            if(mouse_x >= text_x -text_w/2 && mouse_x <=text_x + text_w/2// si ca marche plus enlever les /2
                && mouse_y >= text_y && mouse_y  <= text_y + text_h) {   //et le premier-text_w
                destroy_bitmap(win);
                allegro_exit();
                return;
            }
            if(mouse_x >= text_x - text_w/2 && mouse_x <=text_x + text_w/2// si ca marche plus enlever les /2
                && mouse_y >= text_y-40 && mouse_y  <= text_y-40 + text_h) {   //et le premier-text_w
                destroy_bitmap(win);
                for(int i =0; i <nb_joueurs;i++) {
                    switch (perso[i].classe) {
                        case 0: //Stromtrooper
                            perso[i].pv = 70;
                            perso[i].pa= 2;
                            perso[i].pm = 10;
                            break;
                        case 1: //Sith
                            perso[i].pv = 100;
                            perso[i].pa = 2;
                            perso[i].pm = 20;
                            break;
                        case 2 : //Wookiee
                            perso[i].pv = 120;
                            perso[i].pa = 2;
                            perso[i].pm= 15;
                            break;
                        case 3: //Jedi
                            perso[i].pv = 80;
                            perso[i].pa = 2;
                            perso[i].pm = 25;
                        break;
                    }
                    int x, y;
                    int occupied;

                    do {
                        x = rand() % 12;
                        rest(30);
                        y = rand() % 12;

                        // Vérifier si un autre joueur occupe déjà cette case
                        occupied = 0;
                        for (int j = 0; j < nb_joueurs; j++) {
                            if (j != i
                             && perso[j].x == x
                             && perso[j].y == y)
                            {
                                occupied = 1;
                                break;
                            }
                        }
                    } while (occupied);

                    // Maintenant la case (x,y) est libre
                    perso[i].x = x;
                    perso[i].y = y;
                }
                //creer_plateau(plateau);
                //afficher_plateau(*plateau,perso,nb_joueurs);
                jeu(perso,plateau,nb_joueurs);
                }

        }
        rest(20);
    }

    destroy_bitmap(win);

}

