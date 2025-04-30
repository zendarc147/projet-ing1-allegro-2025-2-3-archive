#include <allegro.h>
#include "moteur.h"

// structure pour les boutons
typedef struct button {
    int x, y, width, height;
    char label[20];
} Button;

void dessiner_bouton(BITMAP *bmp, Button b) {
    // Vérifier survol
    int survole = (mouse_x > b.x && mouse_x < b.x + b.width
                   && mouse_y > b.y && mouse_y < b.y + b.height);
    // Ajustement taille si survol
    int offset = survole ? 5 : 0;
    // Dessiner contour agrandi
    rect(bmp,
         b.x - offset, b.y - offset,
         b.x + b.width + offset, b.y + b.height + offset,
         makecol(255, 255, 255));
    // Dessiner texte centré
    textout_centre_ex(bmp, font,
                      b.label,
                      b.x + b.width/2,
                      b.y + b.height/2 - 4,
                      makecol(255, 255, 255),
                      -1);
}

int menu() {
    BITMAP *fond_ecran;
    Button boutons[3];

    initialisation_allegro();

    buffer = create_bitmap(640, 480);
    fond_ecran = load_bitmap("images/hotplanet.bmp", NULL);
    if (!fond_ecran) {
        allegro_message("Erreur chargement fond d'écran !");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    // Définitions des boutons
    int largeur = 150, hauteur = 40, espace = 20;
    int depart_y = 150, centre_x = (640 - largeur) / 2;

    strcpy(boutons[0].label, "Jouer");
    boutons[0].x = centre_x;
    boutons[0].y = depart_y;
    boutons[0].width = largeur;
    boutons[0].height = hauteur;

    strcpy(boutons[1].label, "Rejouer");
    boutons[1].x = centre_x;
    boutons[1].y = depart_y + hauteur + espace;
    boutons[1].width = largeur;
    boutons[1].height = hauteur;

    strcpy(boutons[2].label, "Quitter");
    boutons[2].x = centre_x;
    boutons[2].y = depart_y + 2 * (hauteur + espace);
    boutons[2].width = largeur;
    boutons[2].height = hauteur;

    show_mouse(NULL);

    while (!key[KEY_ESC]) {
        // Affichage fond et titre
        stretch_blit(fond_ecran, buffer,
                     0, 0, fond_ecran->w, fond_ecran->h,
                     0, 0, 640, 480);
        textout_centre_ex(buffer, font,
                          "ECE Arena", 640/2, 50,
                          makecol(255,255,255), -1);
        // Dessiner boutons
        for (int i = 0; i < 3; i++) {
            dessiner_bouton(buffer, boutons[i]);
        }
        // Curseur et swap
        draw_sprite(buffer, mouse_sprite, mouse_x, mouse_y);
        blit(buffer, screen, 0, 0, 0, 0, 640, 480);

        if (mouse_b & 1) {
            int selected = -1;
            // Détecter bouton cliqué
            for (int i = 0; i < 3; i++) {
                if (mouse_x >= boutons[i].x && mouse_x < boutons[i].x + boutons[i].width
                    && mouse_y >= boutons[i].y && mouse_y < boutons[i].y + boutons[i].height) {
                    selected = i;
                    break;
                }
            }

            if (selected != -1) {
                // Actions selon le bouton sélectionné
                switch (selected) {
                    case 0: { // Jouer : afficher vaisseau.bmp
                        BITMAP *vaisseau = load_bitmap("images/vaisseau.bmp", NULL);
                        if (!vaisseau) {
                            allegro_message("Erreur chargement vaisseau.bmp !");
                        } else {
                            // Afficher l'image sur tout l'écran
                            blit(vaisseau, screen,
                                 0, 0, 0, 0,
                                 vaisseau->w, vaisseau->h);
                            // Attendre une touche
                            readkey();
                            destroy_bitmap(vaisseau);
                        }
                        break;
                    }
                    case 1: // Rejouer
                        allegro_message("Rejouer");
                        break;
                    case 2: // Quitter
                        destroy_bitmap(buffer);
                        destroy_bitmap(fond_ecran);
                        allegro_exit();
                        return 0;
                }
            }
            // Attendre relâchement
            while (mouse_b & 1) rest(10);
        }
        rest(20);
    }

    destroy_bitmap(buffer);
    destroy_bitmap(fond_ecran);
    allegro_exit();
    return 0;
}
