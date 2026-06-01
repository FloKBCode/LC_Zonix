// player.c - deplacement, trail, mort — taille dynamique + garde-fou overflow
#include <string.h>
#include "../include/player.h"
#include "../include/map.h"
#include "../include/constants.h"

#define MAX_TRAIL (GRID_W * GRID_H)

void player_init(Player *p, int x, int y, Direction dir) {
    p->x=x; p->y=y; p->dir=dir; p->alive=1; p->score=0;
    p->trail_len=0; p->returning=0; p->shield_ticks=0;
    p->speed_ticks=0; p->move_skip=0;
    p->kills=0; p->captures=0; p->max_territory=0;
    memset(p->trail, 0, sizeof(p->trail));
}

int player_trail_contains(Player *p, int nx, int ny) {
    for (int t = 0; t < p->trail_len; t++)
        if (p->trail[t][0]==nx && p->trail[t][1]==ny) return 1;
    return 0;
}

void player_claim_trail(GameState *gs, int player_id) {
    Player *p = &gs->players[player_id-1];
    for (int t = 0; t < p->trail_len; t++)
        map_set(gs, p->trail[t][0], p->trail[t][1], player_id);
    p->trail_len = 0; p->returning = 0;
}

void player_clear_trail(GameState *gs, int player_id) {
    Player *p = &gs->players[player_id-1];
    for (int t = 0; t < p->trail_len; t++) {
        int x=p->trail[t][0], y=p->trail[t][1];
        int cur = map_get(gs, x, y);
        if (cur==0 || cur==player_id) map_set(gs, x, y, 0);
    }
    p->trail_len = 0;
}

int player_move(GameState *gs, int player_id) {
    Player *p = &gs->players[player_id-1];
    if (!p->alive) return 0;

    int nx=p->x, ny=p->y;
    switch (p->dir) {
        case DIR_UP:    ny--; break;
        case DIR_DOWN:  ny++; break;
        case DIR_LEFT:  nx--; break;
        case DIR_RIGHT: nx++; break;
    }

    if (!map_in_bounds_gs(gs, nx, ny)) {
        if (!p->shield_ticks) player_kill(gs, player_id);
        return 0;
    }

    int on_own = (map_get(gs, nx, ny) == player_id);

    if (player_trail_contains(p, nx, ny)) {
        if (!p->shield_ticks) player_kill(gs, player_id);
        return 0;
    }

    p->x = nx; p->y = ny;

    if (on_own && p->trail_len > 0) return 1;

    if (!on_own) {
        // AUDIT CRITIQUE: garde-fou overflow trail
        if (p->trail_len >= MAX_TRAIL) {
            player_kill(gs, player_id);
            return 0;
        }
        p->trail[p->trail_len][0] = nx;
        p->trail[p->trail_len][1] = ny;
        p->trail_len++;
        p->returning = 0;
    } else {
        p->returning = 1;
    }
    return 0;
}

void player_kill(GameState *gs, int player_id) {
    player_clear_trail(gs, player_id);
    gs->players[player_id-1].alive = 0;
}