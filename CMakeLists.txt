cmake_minimum_required(VERSION 3.24)
project(allegro_sprite C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    include(ExternalProject)
    find_package(X11 REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(ALSA REQUIRED alsa)
    set(ALLEGRO_PREFIX "${CMAKE_BINARY_DIR}/allegro")
    ExternalProject_Add(allegro4
        URL https://github.com/liballeg/allegro5/releases/download/4.4.3.1/allegro-4.4.3.1.tar.gz
        URL_HASH SHA256=ec19dbc9a021244582b4819b3583ee594b50141f9fcf6944a4ed8069cbf8d4d4
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=${ALLEGRO_PREFIX}
            -DCMAKE_BUILD_TYPE=Release
            -DCMAKE_C_STANDARD=90
            -DWANT_MODULES=OFF
            -DWANT_ALLEGROGL=OFF -DWANT_LOADPNG=OFF
            -DWANT_LOGG=OFF -DWANT_JPGALLEG=OFF
            -DWANT_EXAMPLES=OFF -DWANT_TOOLS=OFF -DWANT_TESTS=OFF -DWANT_DOCS=OFF
        BUILD_BYPRODUCTS "${ALLEGRO_PREFIX}/lib/liballeg.so"
        LOG_BUILD ON
        LOG_INSTALL ON
        LOG_OUTPUT_ON_FAILURE ON
    )
endif()

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
        tableau_scores.c
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_dependencies(allegro_sprite allegro4)
    target_include_directories(allegro_sprite SYSTEM PRIVATE "${ALLEGRO_PREFIX}/include")
    target_link_libraries(allegro_sprite PRIVATE "${ALLEGRO_PREFIX}/lib/liballeg.so")
elseif(WIN32)
    target_link_libraries(allegro_sprite PRIVATE mingw32 alleg44)
else()
    message(FATAL_ERROR "Cette restauration est prévue pour Linux ou MinGW sous Windows.")
endif()
