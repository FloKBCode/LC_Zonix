// game.c - logique principale avec countdown, bouclier spawn, powerups, stats
#include <string.h>
#include "../include/game.h"
#include "../include/map.h"
#include "../include/player.h"
#include "../include/flood_fill.h"
#include "../include/powerup.h"
#include "../include/constants.h"

#define START_MARGIN     6
#define TERRITORY_RADIUS 4

static const int START_X_PCT[4] = {15, 85, 15, 85}; // % de grid_w
static const int START_Y_PCT[4] = {15, 15, 85, 85}; // % de grid_h
static const Direction START_DIR[4] = { DIR_RIGHT, DIR_LEFT, DIR_RIGHT, DIR_LEFT };

void game_init(GameState *gs, int player_count, int grid_w, int grid_h) {
    // Garde les pseudos et couleurs déjà configurés
    char pseudos[MAX_PLAYERS][16];
    int  colors[MAX_PLAYERS];
    for (int i = 0; i < MAX_PLAYERS; i++) {
        memcpy(pseudos[i], gs->players[i].pseudo, 16);
        colors[i] = gs->players[i].color_idx;
    }

    memset(gs, 0, sizeof(*gs));
    gs->player_count = player_count;
    gs->running      = 1;
    gs->ticks        = 0;
    gs->countdown    = COUNTDOWN_TICKS;
    gs->grid_w       = grid_w  > 0 ? grid_w  : GRID_W;
    gs->grid_h       = grid_h  > 0 ? grid_h  : GRID_H;

    map_init(gs);

    for (int i = 0; i < player_count; i++) {
        int sx = (START_X_PCT[i] * gs->grid_w) / 100;
        int sy = (START_Y_PCT[i] * gs->grid_h) / 100;
        player_init(&gs->players[i], sx, sy, START_DIR[i]);

        // Restaure pseudo et couleur
        memcpy(gs->players[i].pseudo, pseudos[i], 16);
        gs->players[i].color_idx = colors[i];

        // Bouclier de spawn
        gs->players[i].shield_ticks = SPAWN_SHIELD_TICKS;

        // Territoire initial
        for (int dy = -TERRITORY_RADIUS; dy <= TERRITORY_RADIUS; dy++)
            for (int dx = -TERRITORY_RADIUS; dx <= TERRITORY_RADIUS; dx++)
                map_set(gs, sx+dx, sy+dy, i+1);
    }

    powerup_init(gs);
    map_calc_scores(gs);
}

void game_update(GameState *gs) {
    // Countdown avant début
    if (gs->countdown > 0) {
        gs->countdown--;
        return;
    }

    gs->ticks++;
    powerup_update(gs);

    for (int i = 0; i < gs->player_count; i++) {
        Player *p = &gs->players[i];
        if (!p->alive) continue;

        // Vitesse : avec boost on bouge à chaque tick,
        // sans boost on saute un tick sur deux (vitesse normale)
        if (p->speed_ticks == 0) {
            // vitesse normale : saute un tick sur deux
            p->move_skip = !p->move_skip;
            if (p->move_skip) continue;
        }

        // Calcule la prochaine case
        int nx = p->x, ny = p->y;
        switch (p->dir) {
            case DIR_UP:    ny--; break;
            case DIR_DOWN:  ny++; break;
            case DIR_LEFT:  nx--; break;
            case DIR_RIGHT: nx++; break;
        }

        // Collision bord
        if (nx < 0 || nx >= gs->grid_w || ny < 0 || ny >= gs->grid_h) {
            if (!p->shield_ticks) player_kill(gs, i+1);
            continue;
        }

        // Collision trail ennemi -> tue l'ennemi
        for (int j = 0; j < gs->player_count; j++) {
            if (j == i || !gs->players[j].alive) continue;
            if (player_trail_contains(&gs->players[j], nx, ny)) {
                if (!gs->players[j].shield_ticks) {
                    player_kill(gs, j+1);
                    p->kills++;
                }
            }
        }

        // Déplace
        int closed = player_move(gs, i+1);
        if (closed) {
            player_claim_trail(gs, i+1);
            flood_fill_capture(gs, i+1);
            p->captures++;
        }

        // Ramasse power-up
        powerup_check_pickup(gs, i+1);

        // Suicide sur son propre trail = mort sauf bouclier
        // (géré dans player_move déjà, mais ici on vérifie le bouclier)
    }

    // Collisions tête-à-tête
    for (int i = 0; i < gs->player_count; i++) {
        if (!gs->players[i].alive) continue;
        for (int j = i+1; j < gs->player_count; j++) {
            if (!gs->players[j].alive) continue;
            if (gs->players[i].x == gs->players[j].x &&
                gs->players[i].y == gs->players[j].y) {
                int si = gs->players[i].shield_ticks;
                int sj = gs->players[j].shield_ticks;
                if (!si) player_kill(gs, i+1);
                if (!sj) player_kill(gs, j+1);
            }
        }
    }

    // Stats max territoire
    map_calc_scores(gs);
    for (int i = 0; i < gs->player_count; i++) {
        if (gs->players[i].score > gs->players[i].max_territory)
            gs->players[i].max_territory = gs->players[i].score;
    }

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
    int best_id = 0, best_score = -1;
    for (int i = 0; i < gs->player_count; i++) {
        if (gs->players[i].score > best_score) {
            best_score = gs->players[i].score;
            best_id    = i+1;
        }
    }
    return best_id;
}

void game_build_stats(GameState *gs, EndStats *stats) {
    stats->winner_id = game_winner(gs);
    for (int i = 0; i < gs->player_count; i++) {
        stats->scores[i]       = gs->players[i].score;
        stats->kills[i]        = gs->players[i].kills;
        stats->captures[i]     = gs->players[i].captures;
        stats->max_territory[i]= gs->players[i].max_territory;
        memcpy(stats->pseudos[i], gs->players[i].pseudo, 16);
    }
    stats->grid_w = gs->grid_w > 0 ? gs->grid_w : GRID_W;
    stats->grid_h = gs->grid_h > 0 ? gs->grid_h : GRID_H;
    // pour ne pas executer la fermeture de boucle deux fois :
    }
