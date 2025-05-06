#include "moteur.h"

void afficher_plateau(int *plateau, int X, int Y) {
    int x = 0, minXY = X < Y ? X : Y;
    float size = 6.0 / (X + Y);
    size = size < 1.0 ? size : 1.0;
    BITMAP *cases = load_bitmap("images/plateau/case.bmp", NULL);
    if (!cases) {
        printf("Erreur chargement image\n");
        exit(1);
    }

    for (int y = 0; y < X + Y; y++) {
        for (int i = 0; i < y && i < minXY && i < X + Y - y; i++) {
            stretch_sprite(buffer, cases, (x - 2 * i) * 100 * size, (y) * 62 * size, cases->w * size, cases->h * size);
        }
        y < X ? x++ : x--;
    }
    printf("Fin\n");
    blit(buffer, screen, 0, 0, 0, 0, 640, 480);
    while (!key[KEY_ESC]);

    destroy_bitmap(cases);
}