# ECE Arena — restauration du projet Allegro

Le jeu reste en C avec Allegro 4.4.3.1. Les images, polices, musiques, personnages,
attaques et menus originaux sont conservés. Les ressources restent dans
`cmake-build-debug/images` et `cmake-build-debug/son` : ne pas supprimer ce dossier.

## Lancer le jeu sous Linux

Depuis le dossier du projet :

```sh
uv run lancer.py
```

Au premier lancement, `uv` installe les versions verrouillées de CMake et Ninja.
CMake télécharge la version officielle d'Allegro 4, vérifie son SHA-256 et la
compile dans `build/`. Aucune installation globale d'Allegro n'est nécessaire.
Les lancements suivants réutilisent la compilation. Une connexion Internet est
nécessaire seulement pour télécharger les dépendances manquantes.

Le compilateur C/C++, `pkg-config`, les en-têtes X11 et ALSA sont requis. Ils sont
déjà présents sur la machine utilisée pour cette restauration. Sur une autre
machine Arch Linux :

```sh
sudo pacman -S --needed base-devel pkgconf libx11 libxext libxpm libxxf86vm libxcursor alsa-lib
```

Sur Debian/Ubuntu :

```sh
sudo apt install build-essential pkg-config libx11-dev libxext-dev libxpm-dev libxxf86vm-dev libxcursor-dev libasound2-dev
```

Pour compiler seulement : `uv run lancer.py --build-only`.

## Jouer

- **Jouer** : choisir 2, 3 ou 4 joueurs, puis un personnage par joueur.
- **MOVE** : sélectionner une case verte pour se déplacer.
- **S1–S4** : choisir une attaque, puis une case rouge.
- **coup** : attaquer une case voisine.
- **fin du tour** : passer au joueur suivant ; le timer dure 15 secondes.
- **Échap**, la fermeture de fenêtre ou **Quitter** : quitter le jeu.
- **rejouer**, après la victoire : relancer avec les mêmes personnages.

Le bouton **Rejouer** du menu initial était vide dans le projet d'origine ; il
reste inchangé. La relance depuis l'écran de victoire est fonctionnelle.

## Corrections limitées

La configuration Windows seule a été adaptée à Linux. Les corrections du moteur
concernent les sorties du jeu, les accès mémoire lors des clics hors plateau,
les animations qui libéraient le plateau, les joueurs non initialisés, la relance
après victoire et le timer (temps réel au lieu du temps CPU). Le rendu conserve
les ressources originales ; aucun équilibrage des attaques n'a été effectué.

## Vérifier

```sh
uv run lancer.py --build-only
uv run tests/verifier.py
```

Les tests Linux demandent `Xvfb` et les en-têtes de `libXtst` (Arch :
`xorg-server-xvfb libxtst` ; Debian/Ubuntu : `xvfb libxtst-dev`). Ils ouvrent les
vraies fenêtres Allegro dans un affichage isolé et simulent les clics. Ils
vérifient les menus à 2/4 joueurs, le déplacement, les attaques, les quatre
animations, la victoire, la relance, les clics hors plateau et le timer.
AddressSanitizer et UndefinedBehaviorSanitizer détectent les erreurs mémoire et
les comportements indéfinis dans le code du jeu. La détection des fuites est
désactivée pour les allocations internes des bibliothèques graphiques.

La restauration a été vérifiée sous Linux/X11 (XWayland sur un bureau Wayland).
La branche de compilation MinGW reste présente ; Windows n'a pas été testé.

Références : [Allegro 4 avec CMake](https://liballeg.org/stabledocs/en/build/cmake.html),
[gestion des projets uv](https://docs.astral.sh/uv/guides/projects/).
