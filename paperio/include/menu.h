// menu.h - ecrans menu, configuration reseau, tutoriel
#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include "types.h"

// Resultat d'un ecran de menu
typedef enum {
    MENU_RESULT_NONE,
    MENU_RESULT_SERVER,   // l'utilisateur veut lancer le serveur
    MENU_RESULT_CLIENT,   // l'utilisateur veut se connecter
    MENU_RESULT_QUIT,     // quitter
    MENU_RESULT_BACK,     // retour au menu precedent
} MenuResult;

// Stocke les parametres reseau saisis par l'utilisateur
typedef struct {
    char ip[64];          // IP du serveur (pour le client)
    int  port;            // port (defaut 5555)
    int  player_count;    // nb de joueurs attendus (pour le serveur)
} NetworkConfig;

// --- Ecrans ---

// Ecran principal : choix Serveur / Client / Quitter
// Bloque jusqu'a ce que l'utilisateur fasse un choix
MenuResult menu_main(SDL_Renderer *ren);

// Ecran de configuration reseau (saisie IP + port)
// Remplit cfg et retourne MENU_RESULT_SERVER ou MENU_RESULT_CLIENT
MenuResult menu_network(SDL_Renderer *ren, NetworkConfig *cfg, int is_server);

// Ecran tutoriel (controls + regles du jeu)
// Retourne MENU_RESULT_BACK quand l'utilisateur appuie sur une touche
void menu_tutorial(SDL_Renderer *ren);

// Ecran "en attente de joueurs" (affiche le nb de joueurs connectes)
void menu_waiting(SDL_Renderer *ren, int connected, int expected);

// Ecran "connexion en cours..."
void menu_connecting(SDL_Renderer *ren, const char *ip, int port);

#endif
