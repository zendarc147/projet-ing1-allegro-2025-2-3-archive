#include "moteur.h"
#define EPAISSEUR_CASE 0.8

void creer_plateau(t_plateau *plateau) {
    int x, y;
    // Charger les textures
    BITMAP *cases = load_bitmap("images/plateau/case.bmp", NULL);

    // Determiner la taille de la bitmap
    x = cases->w * (plateau->x + plateau->y) / 2;
    y = cases->h * EPAISSEUR_CASE * (plateau)

    if (plateau->bitmap != NULL) destroy_bitmap(plateau->bitmap); // Attention à ne pas libérer en dehors de cette fonction

    create_bitmap(x, y);
}

void afficher_plateau(int* plateau, int X, int Y) {
    int x = 0, minXY = X < Y ? X : Y;
    int a = 0, b = 0;
    float size = 6.0 / (X + Y);
    size = size < 1.0 ? size : 1.0;
    BITMAP *cases = load_bitmap("images/plateau/case.bmp", NULL);
    if (!cases) {
        printf("Erreur chargement image\n");
        exit(1);
    }

    for (int y = 0; y < X + Y; y++) {
        for (int i = 0; i <= y && i < minXY && i < X + Y - y - 1; i++) {
            printf("x : %d, y : %d, case : %d\n", a, b, a + X * b);
            switch (plateau[a + X * b]) {
                case 1:
                    stretch_sprite(buffer, cases, 320 + (x - 2 * i - (X - Y + 2.25) / 2) * 100 * size, 240 + (y - (X + Y) / 2) * 62 * size, cases->w * size, cases->h * size);
                    break;
            }
            a--; b++;
        }
        if (y < X - 1) {
            x++;
            a = y + 1;
            b = 0;
        } else {
            x--;
            a = X - 1;
            b = y - X + 2;
        }
    }
    printf("Fin\n");
    blit(buffer, screen, 0, 0, 0, 0, 640, 480);
    while (!key[KEY_ESC]);

    destroy_bitmap(cases);
}

void printPlateau(int plateau[20][20], int x, int y) {
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            printf("%d ", plateau[i][j]);
        }
        printf("\n");
    }
}