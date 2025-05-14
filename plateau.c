#include "moteur.h"
#define EPAISSEUR_CASE 0.75

void creer_plateau(t_plateau *plateau) {
    int w, h, X = 0, Y = 0;
    float x, y = 0;
    // Charger les textures
    BITMAP *cases = load_bitmap("images/plateau/case.bmp", NULL);
    BITMAP *cases_rouge = load_bitmap("images/plateau/case_rouge.bmp", NULL);
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
        // calcul du point de départ sur la diagonale i
        int startX = (i < plateau->x) ? i : plateau->x - 1;
        int startY = (i < plateau->y) ? 0 : i - (plateau->y - 1);

        // longueur de la diagonale
        int len = plateau->x + plateau->y - 1 - i;
        if (len > plateau->x) len = plateau->x;
        if (len > plateau->y) len = plateau->y;

        // on stocke X et Y au début
        int X = startX;
        int Y = startY;

        for (int j = 0; j < len; j++) {
            // on dessine la case (X,Y)
            if (plateau->plateau[Y][X] < 100) {
                stretch_sprite(plateau->bitmap, cases,
                               x - j * cases->w, y,
                               cases->w, cases->h);
            }
            else {
                stretch_sprite(plateau->bitmap, cases_rouge,
                               x - j * cases->w, y,
                               cases->w, cases->h);
            }
            // on décale X et Y sans écrire X = X - j, Y = startY + j
            X--;  // descend d’une colonne à gauche sur la diagonale
            Y++;  // monte d’une ligne sur la diagonale
        }

        // ajustement de x,y comme avant…
        if (i < plateau->y - 1) {
            x += cases->w / 2;
        } else {
            x -= cases->w / 2;
        }
        y += cases->h * EPAISSEUR_CASE / 2;
    }

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