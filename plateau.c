#include "moteur.h"
#define EPAISSEUR_CASE 0.75

void creer_plateau(t_plateau *plateau) {
    int w, h;
    float x, y = 0;
    // Charger les textures
    BITMAP *cases = load_bitmap("images/plateau/case.bmp", NULL);
    if (cases == NULL) {
        allegro_message("Erreur lors du chargement de l'image");
        exit(EXIT_FAILURE);
    }

    // Determiner la taille de la bitmap
    w = cases->w * (plateau->x + plateau->y) / 2;
    h = cases->h * EPAISSEUR_CASE * (plateau->x + plateau->y) / 2 + cases->h * (1 - EPAISSEUR_CASE);

    plateau->taille = 640.0 / w;

    if (plateau->bitmap != NULL) destroy_bitmap(plateau->bitmap); // Attention à ne pas libérer en dehors de cette fonction

    plateau->bitmap = create_bitmap(w, h);

    // Determine la position de la premiere case
    x = plateau->bitmap->w / 2 + cases->w * (plateau->x - plateau->y - 2) / 4;

    for (int i = 0; i < plateau->x + plateau->y - 1; i++) {
        for (int j = 0; j <= i &&
            j < (plateau->x < plateau->y ? plateau->x : plateau->y)
            && j < plateau->x + plateau->y - i - 1; j++) {
            stretch_sprite(plateau->bitmap, cases, x - j * cases->w, y, cases->w, cases->h);
        }
        if (i < plateau->y - 1) x += cases->w / 2;
        else x -= cases->w / 2;
        y += cases->h * EPAISSEUR_CASE / 2;
    }
}

void afficher_plateau(t_plateau plateau) {
    stretch_sprite(buffer, plateau.bitmap, plateau.dx, plateau.dy, plateau.bitmap->w * plateau.taille, plateau.bitmap->h * plateau.taille);
}

void printPlateau(int plateau[20][20], int x, int y) {
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            printf("%d ", plateau[i][j]);
        }
        printf("\n");
    }
}