# C_Chess

Un moteur de <a href="https://www.youtube.com/@MarcQuenehen">jeu d'échec</a> développé en C, avec la librairie SDL. </br>
Avec toutes les règles implémentées (coups réguliers, roques, prise en passant, promotion) et un joueur ordinateur.

<img src="./assets/readme/illustration.png" target="_blank" alt="chess_illustration" width="500"/>

## Compilation

### Prérequis 

SDL2, SDL2_image, et SDL2_TTF.

```sh
sudo apt update
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```

### Lancer le programme

- Utiliser le makefile avec la commande `make` pour compiler et exécuter `c_chess`.
- Ou lancer le script `compile.sh` qui compile, exécute, puis nettoie les fichiers binaires.

```sh
./compile.sh
```

## Note de développement

Commande pratique pour retrouver les erreurs de segmentation : 
```sh
valgrind --leak-check=full ./c_chess
```
