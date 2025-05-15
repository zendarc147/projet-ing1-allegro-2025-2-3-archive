#include "moteur.h"
#define EPAISSEUR_CASE 0.75

void creer_plateau(t_plateau *plateau) {
    // 1) Chargement des textures
    BITMAP *cases       = load_bitmap("images/plateau/case.bmp",       NULL);
    BITMAP *cases_rouge = load_bitmap("images/plateau/case_rouge.bmp", NULL);
    if (!cases || !cases_rouge) {
        allegro_message("Erreur chargement texture plateau");
        exit(EXIT_FAILURE);
    }

    // 2) Calcul de la taille en pixels (avant mise à l'échelle)
    int tile_w = cases->w;
    int tile_h = cases->h;
    int map_w  = plateau->x;
    int map_h  = plateau->y;
    // Largeur totale en tuiles isométriques : map_w + map_h
    int pix_w = (map_w + map_h) * tile_w / 2;
    int pix_h = (map_w + map_h) * (int)(tile_h * EPAISSEUR_CASE) / 2
              + tile_h * (1 - EPAISSEUR_CASE);

    // 3) Détermination du facteur d'échelle pour tenir dans 640px
    plateau->taille = 640.0f / pix_w;

    // 4) (Re)création du buffer isométrique
    if (plateau->bitmap) destroy_bitmap(plateau->bitmap);
    // On crée exactement pix_w×pix_h, l'échelle sera appliquée à l'affichage
    plateau->bitmap = create_bitmap(pix_w, pix_h);

    // 5) Origine de dessin (centre horizontal, en haut)
    float ox = (pix_w - tile_w) * 0.5f;
    float oy = 0;

    // 6) Remplissage case par case
    for (int row = 0; row < map_h; row++) {
        for (int col = 0; col < map_w; col++) {
            // Position isométrique (avant mise à l'échelle)
            float px = ox + (col - row) * (tile_w  * 0.5f);
            float py = oy + (col + row) * (tile_h * EPAISSEUR_CASE * 0.5f);

            // Choix de la tuile rouge ou normale
            BITMAP *tex = (plateau->plateau[row][col] < 100)
                          ? cases
                          : cases_rouge;

            // On dessine **sans** mise à l’échelle : stretch_sprite sur la bitmap brute
            stretch_sprite(plateau->bitmap, tex,
                           (int)px, (int)py,
                           tile_w, tile_h);
        }
    }

    // 7) On libère les bitmaps temporaires
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