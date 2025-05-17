#include "moteur.h"
#define EPAISSEUR_CASE 0.75

void creer_plateau(t_plateau *plateau) {
    // 1) charger les bitmaps
    BITMAP *cases       = load_bitmap("images/plateau/case.bmp",        NULL);
    BITMAP *cases_rouge = load_bitmap("images/plateau/case_rouge.bmp", NULL);
    BITMAP *cases_verte = load_bitmap("images/plateau/case_verte.bmp", NULL);
    BITMAP *fond = load_bitmap("images/plateau/fond_jeu.bmp",NULL);
    if (!cases || !cases_rouge) {
        allegro_message("Erreur chargement texture plateau");
        exit(EXIT_FAILURE);
    }

    // 2) dimensions “brutes” de la carte isométrique
    int tile_w = cases->w;
    int tile_h = cases->h;
    int map_w  = plateau->x;
    int map_h  = plateau->y;
    int bmp_w = tile_w * (map_w + map_h) / 2;
    int bmp_h = (int)(tile_h * EPAISSEUR_CASE * (map_w + map_h) / 2
                     + tile_h * (1 - EPAISSEUR_CASE));

    // 3) facteur d’échelle pour tenir dans 640px de large
    plateau->taille = 1000.0f / bmp_w;



    // 4) (re)création du buffer isométrique
    if (plateau->bitmap) destroy_bitmap(plateau->bitmap);
    plateau->bitmap = create_bitmap(bmp_w, bmp_h);

    // 5) origine du dessin : centré horizontalement, haut de la bitmap
    float originX = (bmp_w - tile_w) / 2.0f;
    float originY = 0;

    stretch_blit(fond,plateau->bitmap,0,0,fond->w,fond->h,0,0,3000,2000);

    // 6) parcours ligne × colonne
    for (int row = 0; row < map_h; row++) {
        for (int col = 0; col < map_w; col++) {
            // projection isométrique avant échelle
            float px = originX + (col - row) * (tile_w  * 0.5f);
            float py = originY + (col + row) * (tile_h * EPAISSEUR_CASE * 0.5f);

            int sw = tile_w * plateau->taille;
            int sh = tile_h * plateau->taille;

            int cx = (int)(px * plateau->taille + sw / 2 + plateau->dx);
            int cy = (int)(py * plateau->taille + sh * EPAISSEUR_CASE / 2 + plateau->dy);

            int points[8] = {
                cx,              cy - (int)(sh * EPAISSEUR_CASE / 2),
                cx + sw / 2,     cy,
                cx,              cy + (int)(sh * EPAISSEUR_CASE / 2),
                cx - sw / 2,     cy
            };

            int color = makecol(row, col, 0);


            polygon(mask,4, points, color);
            floodfill(mask, cx, cy, color);

            // dessin normal ou rouge selon valeur du plateau
            if (plateau->plateau[row][col] < 100) {
                stretch_sprite(plateau->bitmap, cases,
                               (int)px, (int)py,
                               tile_w, tile_h);
            }
            else if(plateau->plateau[row][col] < 200) {
                stretch_sprite(plateau->bitmap, cases_verte,
                               (int)px, (int)py,
                               tile_w, tile_h);
            }
            else if(plateau->plateau[row][col] < 300) {
                stretch_sprite(plateau->bitmap, cases_rouge,
                               (int)px, (int)py,
                               tile_w, tile_h);
            }
        }
    }

    // 7) libération des bitmaps temporaires
    destroy_bitmap(cases);
    destroy_bitmap(cases_rouge);
}

void afficher_plateau(t_plateau plateau) {
    stretch_sprite(buffer, plateau.bitmap, plateau.dx, plateau.dy, plateau.bitmap->w * plateau.taille, plateau.bitmap->h * plateau.taille);
}

void printPlateau(int plateau[20][20], int x, int y) {
    // 1) Déterminer dynamiquement la largeur nécessaire (nombre de chiffres max)
    int max_val = 0;
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            int v = plateau[i][j] < 0 ? -plateau[i][j] : plateau[i][j];
            if (v > max_val) max_val = v;
        }
    }
    int width = 1;
    while (max_val >= 10) {
        max_val /= 10;
        width++;
    }
    // on ajoute 1 espace pour la séparation
    width += 1;

    // 2) Affichage avec largeur fixe
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            printf("%*d", width, plateau[i][j]);
        }
        printf("\n");
    }
}

int case_survolee(t_plateau plateau) {
    BITMAP* cases = load_bitmap("images/plateau/case.bmp", NULL);
    int case_survolee;
    float X_relatif, Y_relatif, W_relatif, H_relatif;
    W_relatif = (float)cases->w * plateau.taille;
    H_relatif = (float)cases->h * EPAISSEUR_CASE * plateau.taille;
    X_relatif = ((float)mouse_x - plateau.dx - (float)plateau.y * W_relatif / 2) / W_relatif + ((float)mouse_y - plateau.dy) / H_relatif;
    Y_relatif = ((float)mouse_y - plateau.dy) / H_relatif - ((float)mouse_x - plateau.dx - (float)plateau.y * W_relatif / 2) / W_relatif;

    case_survolee = (int)X_relatif + (int)(Y_relatif) * plateau.x;

    destroy_bitmap(cases);
    return case_survolee;
}

int coor_case(t_plateau plateau, int case_x, int case_y) {
    BITMAP* cases = load_bitmap("images/plateau/case.bmp", NULL);
    int x, y;
    x = cases->w / 2 * (case_x - case_y + plateau.y) * plateau.taille + plateau.dx;
    y = cases->h * EPAISSEUR_CASE / 2 * (case_x + case_y + 1) * plateau.taille + plateau.dy;

    destroy_bitmap(cases);
    return x + y * plateau.x;
}