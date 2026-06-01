// player.c - deplacement, trail, mort, collision
#include <string.h>
#include "../include/player.h"
#include "../include/map.h"
#include "../include/constants.h"

void player_init(Player *p, int x, int y, Direction dir) {
    p->x         = x;
    p->y         = y;
    p->dir       = dir;
    p->alive     = 1;
    p->score     = 0;
    p->trail_len = 0;
    p->returning = 0;
    memset(p->trail, 0, sizeof(p->trail));
}

int player_trail_contains(Player *p, int nx, int ny) {
    for (int t = 0; t < p->trail_len; t++)
        if (p->trail[t][0] == nx && p->trail[t][1] == ny)
            return 1;
    return 0;
}

void player_claim_trail(GameState *gs, int player_id) {
    Player *p = &gs->players[player_id - 1];
    for (int t = 0; t < p->trail_len; t++) {
        int x = p->trail[t][0];
        int y = p->trail[t][1];
        map_set(gs, x, y, player_id);
    }
    p->trail_len = 0;
    p->returning = 0;
}

void player_clear_trail(GameState *gs, int player_id) {
    Player *p = &gs->players[player_id - 1];
    // Efface les cases de trail de la carte (remet a 0 si elles n'etaient pas territoire)
    for (int t = 0; t < p->trail_len; t++) {
        int x = p->trail[t][0];
        int y = p->trail[t][1];
        // On remet a 0 seulement si personne d'autre ne revendique cette case
        if (map_get(gs, x, y) == 0 || map_get(gs, x, y) == player_id)
            map_set(gs, x, y, 0);
    }
    p->trail_len = 0;
}

// Retourne 1 si le joueur vient de fermer une boucle (retour sur son territoire)
int player_move(GameState *gs, int player_id) {
    Player *p = &gs->players[player_id - 1];
    if (!p->alive) return 0;

    // Calcule la nouvelle position
    int nx = p->x;
    int ny = p->y;
    switch (p->dir) {
        case DIR_UP:    ny--; break;
        case DIR_DOWN:  ny++; break;
        case DIR_LEFT:  nx--; break;
        case DIR_RIGHT: nx++; break;
    }

    // Collision avec le bord de la grille
    if (!map_in_bounds(nx, ny)) {
        player_kill(gs, player_id);
        return 0;
    }

    // Verifie si la case cible est le propre territoire du joueur
    int on_own_territory = (map_get(gs, nx, ny) == player_id);
    int has_trail = (p->trail_len > 0);

    // Collision avec son propre trail (suicide)
    if (player_trail_contains(p, nx, ny)) {
        player_kill(gs, player_id);
        return 0;
    }

    // Deplace le joueur
    p->x = nx;
    p->y = ny;

    // Si le joueur revient sur son territoire avec un trail -> capturer
    if (on_own_territory && has_trail) {
        return 1;  // signal : lancer flood fill
    }

    // Si le joueur n'est pas sur son territoire -> ajouter au trail
    if (!on_own_territory) {
        p->trail[ p->trail_len ][0] = nx;
        p->trail[ p->trail_len ][1] = ny;
        p->trail_len++;
        p->returning = 0;
    } else {
        // Sur son territoire, pas de trail -> rien de special
        p->returning = 1;
    }

    return 0;
}

void player_kill(GameState *gs, int player_id) {
    Player *p = &gs->players[player_id - 1];
    player_clear_trail(gs, player_id);
    p->alive = 0;
}
