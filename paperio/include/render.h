// render.h - rendu SDL2 en mode 2.5D (shadow offset)
#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include "types.h"

// Initialise la fenetre SDL2 et le renderer
// Retourne 0 si OK, -1 si erreur
int  render_init(SDL_Window **win, SDL_Renderer **ren);

// Dessine l'etat du jeu complet en 2.5D
//   my_id : ID du joueur local (pour le surligner)
void render_draw(SDL_Renderer *ren, GameState *gs, int my_id);

// Libere les ressources SDL2
void render_cleanup(SDL_Window *win, SDL_Renderer *ren);

// ----- Fonctions internes exposees pour les tests -----

// Dessine un "bloc 2.5D" : face du dessus + ombre portee (bas et droite)
// (x_px, y_px) = coin haut-gauche en pixels, sz = taille de la face
// lift = decalage vertical vers le haut pour l'effet d'elevation
void render_block_25d(SDL_Renderer *ren,
                      int x_px, int y_px, int sz, int lift,
                      SDL_Color top_color, SDL_Color shadow_color);

// Dessine un chiffre unique (0-9) en pixel art 3x5 a la position donnee
void render_digit(SDL_Renderer *ren, int x, int y, int digit,
                  SDL_Color col, int scale);

// Dessine un entier (jusqua 4 chiffres) en pixel art
void render_number(SDL_Renderer *ren, int x, int y, int number,
                   SDL_Color col, int scale);

// Dessine un caractere ASCII en pixel art 5x7 a la position donnee
void render_char(SDL_Renderer *ren, int x, int y, char c,
                 SDL_Color col, int scale);

// Dessine une chaine de caracteres en pixel art
void render_string(SDL_Renderer *ren, int x, int y, const char *str,
                   SDL_Color col, int scale);

// Dessine le HUD : scores + joueur local mis en valeur
void render_hud(SDL_Renderer *ren, GameState *gs, int my_id);

// Dessine l'ecran de victoire
void render_game_over(SDL_Renderer *ren, int winner_id, GameState *gs);

#endif
