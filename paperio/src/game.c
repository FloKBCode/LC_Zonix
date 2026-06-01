// game.c - logique principale : initialisation, update par tick, conditions de fin
#include <string.h>
#include "../include/game.h"
#include "../include/map.h"
#include "../include/player.h"
#include "../include/flood_fill.h"
#include "../include/constants.h"

// Positions de depart : 4 coins de la carte avec une petite marge
// et territoire initial 5x5 autour de chaque joueur
#define START_MARGIN 8
#define TERRITORY_RADIUS 4

static const int START_X[4] = {
    START_MARGIN,
    GRID_W - 1 - START_MARGIN,
    START_MARGIN,
    GRID_W - 1 - START_MARGIN
};
static const int START_Y[4] = {
    START_MARGIN,
    START_MARGIN,
    GRID_H - 1 - START_MARGIN,
    GRID_H - 1 - START_MARGIN
};
static const Direction START_DIR[4] = {
    DIR_RIGHT,
    DIR_LEFT,
    DIR_RIGHT,
    DIR_LEFT
};

void game_init(GameState *gs, int player_count) {
    memset(gs, 0, sizeof(*gs));
    gs->player_count = player_count;
    gs->running      = 1;
    gs->ticks        = 0;

    map_init(gs);

    for (int i = 0; i < player_count; i++) {
        player_init(&gs->players[i], START_X[i], START_Y[i], START_DIR[i]);

        // Territoire initial : carré de (2*TERRITORY_RADIUS+1)^2 cases
        int cx = START_X[i];
        int cy = START_Y[i];
        for (int dy = -TERRITORY_RADIUS; dy <= TERRITORY_RADIUS; dy++) {
            for (int dx = -TERRITORY_RADIUS; dx <= TERRITORY_RADIUS; dx++) {
                int tx = cx + dx;
                int ty = cy + dy;
                if (map_in_bounds(tx, ty))
                    map_set(gs, tx, ty, i + 1);  // ID joueur = i+1
            }
        }
    }

    map_calc_scores(gs);
}

void game_update(GameState *gs) {
    gs->ticks++;

    for (int i = 0; i < gs->player_count; i++) {
        Player *p = &gs->players[i];
        if (!p->alive) continue;

        // --- Collision avec le trail d'un AUTRE joueur AVANT de bouger ---
        // (Verifie la case que le joueur va atteindre)
        int nx = p->x, ny = p->y;
        switch (p->dir) {
            case DIR_UP:    ny--; break;
            case DIR_DOWN:  ny++; break;
            case DIR_LEFT:  nx--; break;
            case DIR_RIGHT: nx++; break;
        }

        // Verifie si un autre joueur a son trail sur cette case
        for (int j = 0; j < gs->player_count; j++) {
            if (j == i) continue;
            if (!gs->players[j].alive) continue;
            if (player_trail_contains(&gs->players[j], nx, ny)) {
                // Tuer l'autre joueur dont on touche le trail
                player_kill(gs, j + 1);
            }
        }

        // --- Deplacer le joueur ---
        int closed_loop = player_move(gs, i + 1);

        if (closed_loop) {
            // Le joueur est revenu sur son territoire avec un trail
            player_claim_trail(gs, i + 1);
            flood_fill_capture(gs, i + 1);
        }
    }

    // --- Collisions tete-a-tete (deux joueurs sur la meme case) ---
    for (int i = 0; i < gs->player_count; i++) {
        if (!gs->players[i].alive) continue;
        for (int j = i + 1; j < gs->player_count; j++) {
            if (!gs->players[j].alive) continue;
            if (gs->players[i].x == gs->players[j].x &&
                gs->players[i].y == gs->players[j].y) {
                // Les deux meurent
                player_kill(gs, i + 1);
                player_kill(gs, j + 1);
            }
        }
    }

    // Mise a jour des scores
    map_calc_scores(gs);

    // Si un seul joueur reste vivant, on marque la fin
    if (game_is_over(gs))
        gs->running = 0;
}

int game_is_over(GameState *gs) {
    int alive = 0;
    for (int i = 0; i < gs->player_count; i++)
        if (gs->players[i].alive) alive++;
    return (alive <= 1);
}

int game_winner(GameState *gs) {
    // En cas de fin, le gagnant est celui avec le plus de territoire
    int best_id    = 0;
    int best_score = -1;
    for (int i = 0; i < gs->player_count; i++) {
        if (gs->players[i].score > best_score) {
            best_score = gs->players[i].score;
            best_id    = i + 1;
        }
    }
    return best_id;
}
