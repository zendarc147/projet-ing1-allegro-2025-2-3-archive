cmake_minimum_required(VERSION 3.29)
project(allegro_sprite C)

set(CMAKE_C_STANDARD 23)

add_executable(allegro_sprite main.c
        moteur.h menu.c attaque.h
        attaque.c
        deplacement.c
        plateau.c
        loop_jeu.c
        init_jeu.c
        data.c
        "gerer actions.c"
        "convertir plateau.c"
        animation_perso.c
)

target_link_libraries(allegro_sprite -lmingw32 -lalleg44)

