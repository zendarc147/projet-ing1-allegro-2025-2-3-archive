/* Tests du vrai moteur et de ses fenêtres, avec clics X11 dans Xvfb. */
#define main main_original
#include "../main.c"
#undef main

#include <assert.h>
#include <pthread.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

static const char *scenario;
static int destination_x, destination_y, cible_x, cible_y;

static void attendre(int ms) {
    struct timespec delai = {ms / 1000, (ms % 1000) * 1000000L};
    nanosleep(&delai, NULL);
}

static void clic(Display *display, int x, int y) {
    XTestFakeMotionEvent(display, -1, x, y, 0);
    XFlush(display);
    attendre(100);
    XTestFakeButtonEvent(display, 1, True, 0);
    XFlush(display);
    attendre(100);
    XTestFakeButtonEvent(display, 1, False, 0);
    XFlush(display);
    attendre(400);
}

static void *piloter(void *unused) {
    (void)unused;
    Display *display = XOpenDisplay(NULL);
    assert(display);
    attendre(1000);
    if (!strcmp(scenario, "quitter")) {
        clic(display, 500, 485);
    } else if (!strcmp(scenario, "menu2") || !strcmp(scenario, "menu4")) {
        int nombre = !strcmp(scenario, "menu2") ? 2 : 4;
        clic(display, 500, 315);
        clic(display, 500, nombre == 2 ? 335 : 465);
        for (int i = 0; i < nombre; i++) clic(display, 185 + i * 210, 430);
        clic(display, 263, 740);
        clic(display, 990, 790);
        clic(display, 383, 740);
        clic(display, 990, 790);
        clic(display, 145, 740);
        clic(display, 990, 790);
    } else if (!strcmp(scenario, "combat")) {
        clic(display, 263, 740);
        clic(display, 990, 790);
        clic(display, destination_x, destination_y);
        clic(display, 383, 740);
        clic(display, cible_x, cible_y);
        clic(display, 920, 740);
        clic(display, 920, 740);
        clic(display, 145, 740);
        clic(display, cible_x, cible_y);
    } else if (!strcmp(scenario, "rejouer")) {
        clic(display, 500, 674);
    } else if (!strcmp(scenario, "victoire")) {
        clic(display, 500, 714);
    } else if (!strcmp(scenario, "timer")) {
        attendre(15500);
    }
    if (strcmp(scenario, "quitter") && strcmp(scenario, "rejouer") && strcmp(scenario, "victoire")) {
        unsigned int escape = XKeysymToKeycode(display, XK_Escape);
        XTestFakeKeyEvent(display, escape, True, 0);
        XFlush(display);
        attendre(200);
        XTestFakeKeyEvent(display, escape, False, 0);
        XFlush(display);
    }
    XCloseDisplay(display);
    return NULL;
}

int main(int argc, char **argv) {
    assert(argc == 2);
    scenario = argv[1];
    pthread_t robot;
    assert(XInitThreads());
    if (!strcmp(scenario, "quitter") || !strncmp(scenario, "menu", 4)) {
        assert(pthread_create(&robot, NULL, piloter, NULL) == 0);
        assert(main_original() == 0);
        pthread_join(robot, NULL);
    } else {
        initialisation_allegro();
        t_plateau *plateau = initialisation_plateau();
        assert(plateau);
        creer_plateau(plateau);
        t_perso joueurs[2] = {0};
        for (int i = 0; i < 2; i++) {
            joueurs[i].joueur = i;
            joueurs[i].classe = i == 0 && !strcmp(scenario, "combat") ? 2 : i;
            joueurs[i].pv = 100;
            joueurs[i].pa = 2;
            joueurs[i].ordre_mort = -1;
            for (int j = 0; j < 4; j++) joueurs[i].attaque[j] = *attaques[joueurs[i].classe][j];
        }
        joueurs[0].x = 5; joueurs[0].y = 4;
        joueurs[1].x = 5; joueurs[1].y = 6;
        joueurs[0].attaque[0].pourcent_echec = -1;
        coor_case(*plateau, 5, 5, &destination_x, &destination_y);
        coor_case(*plateau, 6, 5, &cible_x, &cible_y);
        jeu_music = load_sample("son/jeu.wav");
        assert(jeu_music);
        assert(play_sample(jeu_music, 255, 128, 1000, TRUE) >= 0);
        if (!strcmp(scenario, "animations")) {
            BITMAP *original = plateau->bitmap;
            int frames[4] = {4, 11, 8, 13};
            for (int classe = 0; classe < 4; classe++) {
                animation_attaque(*plateau, classe, 5, 5, frames[classe]);
                assert(getpixel(original, 0, 0) == bitmap_mask_color(original));
                afficher_plateau(*plateau, joueurs, 2, -1);
            }
            gerer_attaques(&joueurs[0], joueurs, 2, 5, 19, 19);
            assert(joueurs[1].pv == 100);
        } else {
            if (!strcmp(scenario, "rejouer") || !strcmp(scenario, "victoire")) joueurs[1].pv = 0;
            assert(pthread_create(&robot, NULL, piloter, NULL) == 0);
            int resultat = jeu(joueurs, plateau, 2);
            pthread_join(robot, NULL);
            if (!strcmp(scenario, "combat")) {
                assert(joueurs[0].x == 5 && joueurs[0].y == 5);
                assert(joueurs[1].pv < 90);
                assert(joueurs[0].pa == 5);
            } else if (!strcmp(scenario, "timer")) {
                assert(joueurs[1].pa == 4);
            } else if (!strcmp(scenario, "rejouer")) {
                assert(resultat == 1);
                assert(joueurs[0].pv == 70 && joueurs[1].pv == 100);
                assert(joueurs[1].ordre_mort == -1);
            } else {
                assert(resultat == 0);
            }
        }
        destroy_sample(jeu_music);
        destroy_bitmap(plateau->bitmap);
        free(plateau);
        destroy_bitmap(mask);
        destroy_bitmap(buffer);
        allegro_exit();
    }
    printf("OK : %s\n", scenario);
    return 0;
}
