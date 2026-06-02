// powerup.c - spawn, expiration, pickup et effets des power-ups
#include <stdlib.h>
#include <time.h>
#include "../include/powerup.h"
#include "../include/map.h"
#include "../include/player.h"
#include "../include/constants.h"

void powerup_init(GameState *gs) {
    for (int i = 0; i < MAX_POWERUPS; i++) {
        gs->powerups[i].active = 0;
        gs->powerups[i].type   = PUP_NONE;
    }
    srand((unsigned)time(NULL));
}

// Cherche un slot libre et spawn un power-up aléatoire
static void spawn_one(GameState *gs) {
    // Trouve un slot libre
    int slot = -1;
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (!gs->powerups[i].active) { slot = i; break; }
    }
    if (slot < 0) return;

    // Position aléatoire sur une case libre (pas de territoire)
    int tries = 0;
    int x, y;
    do {
        x = 5 + rand() % (gs->grid_w - 10);
        y = 5 + rand() % (gs->grid_h - 10);
        tries++;
    } while (gs->map[y][x] != 0 && tries < 50);

    if (tries >= 50) return;

    gs->powerups[slot].x        = x;
    gs->powerups[slot].y        = y;
    gs->powerups[slot].type     = (PowerUpType)(1 + rand() % 4); // 1-4
    gs->powerups[slot].active   = 1;
    gs->powerups[slot].lifetime = POWERUP_LIFETIME_TICKS;
}

void powerup_update(GameState *gs) {
    // Spawn périodique
    if (gs->ticks % POWERUP_SPAWN_INTERVAL == 0)
        spawn_one(gs);

    // Expiration
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (!gs->powerups[i].active) continue;
        gs->powerups[i].lifetime--;
        if (gs->powerups[i].lifetime <= 0)
            gs->powerups[i].active = 0;
    }

    // Expiration des effets joueurs
    for (int i = 0; i < gs->player_count; i++) {
        Player *p = &gs->players[i];
        if (p->shield_ticks > 0) p->shield_ticks--;
        if (p->speed_ticks  > 0) p->speed_ticks--;
    }
}

void powerup_check_pickup(GameState *gs, int player_id) {
    Player *p = &gs->players[player_id - 1];
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (!gs->powerups[i].active) continue;
        if (gs->powerups[i].x != p->x || gs->powerups[i].y != p->y) continue;

        // Ramassé !
        gs->powerups[i].active = 0;

        switch (gs->powerups[i].type) {
            case PUP_SPEED:
                p->speed_ticks = SPEED_BOOST_TICKS;
                break;
            case PUP_SHIELD:
                p->shield_ticks = SHIELD_TICKS;
                break;
            case PUP_BOMB:
                powerup_apply_bomb(gs, player_id, p->x, p->y);
                break;
            case PUP_TERRITORY: {
                // Capture un carré 5x5 autour du joueur
                for (int dy = -2; dy <= 2; dy++)
                    for (int dx = -2; dx <= 2; dx++)
                        map_set(gs, p->x+dx, p->y+dy, player_id);
                p->captures++;
                break;
            }
            default: break;
        }
    }
}

void powerup_apply_bomb(GameState *gs, int player_id, int x, int y) {
    // Efface le territoire dans un rayon 4 (remet à 0 sauf son propre territoire)
    for (int dy = -4; dy <= 4; dy++) {
        for (int dx = -4; dx <= 4; dx++) {
            int tx = x + dx, ty = y + dy;
            if (!map_in_bounds(tx, ty)) continue;
            int owner = map_get(gs, tx, ty);
            if (owner != 0 && owner != player_id)
                map_set(gs, tx, ty, 0);
        }
    }
    // Tue aussi les joueurs ennemis dans le rayon 3 si ils ont un trail
    for (int i = 0; i < gs->player_count; i++) {
        if (i + 1 == player_id || !gs->players[i].alive) continue;
        int ex = gs->players[i].x - x;
        int ey = gs->players[i].y - y;
        if (ex*ex + ey*ey <= 9) { // rayon 3
            player_kill(gs, i + 1);
            gs->players[player_id-1].kills++;
        }
    }
}
