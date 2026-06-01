// render.c - rendu SDL2 en mode 2.5D (shadow offset) + HUD pixel art
// Pas de SDL2_ttf, pas de SDL2_image : tout est dessine en SDL2 pur
#include <SDL2/SDL.h>
#include <string.h>
#include <stdio.h>
#include "../include/render.h"
#include "../include/constants.h"
#include "../include/colors.h"
#include "../include/types.h"

// =============================================================================
// POLICE PIXEL ART 5x7 intégrée (ASCII 32-90)
// Chaque caractere est encode sur 5 colonnes de 7 bits (bit0 = ligne du haut)
// =============================================================================
static const unsigned char FONT5x7[][5] = {
    // ' ' (32)
    {0x00,0x00,0x00,0x00,0x00},
    // '!' (33)
    {0x00,0x00,0x5F,0x00,0x00},
    // '"' (34)
    {0x00,0x07,0x00,0x07,0x00},
    // '#' (35)
    {0x14,0x7F,0x14,0x7F,0x14},
    // '$' (36)
    {0x24,0x2A,0x7F,0x2A,0x12},
    // '%' (37)
    {0x23,0x13,0x08,0x64,0x62},
    // '&' (38)
    {0x36,0x49,0x55,0x22,0x50},
    // '\'' (39)
    {0x00,0x05,0x03,0x00,0x00},
    // '(' (40)
    {0x00,0x1C,0x22,0x41,0x00},
    // ')' (41)
    {0x00,0x41,0x22,0x1C,0x00},
    // '*' (42)
    {0x14,0x08,0x3E,0x08,0x14},
    // '+' (43)
    {0x08,0x08,0x3E,0x08,0x08},
    // ',' (44)
    {0x00,0x50,0x30,0x00,0x00},
    // '-' (45)
    {0x08,0x08,0x08,0x08,0x08},
    // '.' (46)
    {0x00,0x60,0x60,0x00,0x00},
    // '/' (47)
    {0x20,0x10,0x08,0x04,0x02},
    // '0' (48)
    {0x3E,0x51,0x49,0x45,0x3E},
    // '1' (49)
    {0x00,0x42,0x7F,0x40,0x00},
    // '2' (50)
    {0x42,0x61,0x51,0x49,0x46},
    // '3' (51)
    {0x21,0x41,0x45,0x4B,0x31},
    // '4' (52)
    {0x18,0x14,0x12,0x7F,0x10},
    // '5' (53)
    {0x27,0x45,0x45,0x45,0x39},
    // '6' (54)
    {0x3C,0x4A,0x49,0x49,0x30},
    // '7' (55)
    {0x01,0x71,0x09,0x05,0x03},
    // '8' (56)
    {0x36,0x49,0x49,0x49,0x36},
    // '9' (57)
    {0x06,0x49,0x49,0x29,0x1E},
    // ':' (58)
    {0x00,0x36,0x36,0x00,0x00},
    // ';' (59)
    {0x00,0x56,0x36,0x00,0x00},
    // '<' (60)
    {0x08,0x14,0x22,0x41,0x00},
    // '=' (61)
    {0x14,0x14,0x14,0x14,0x14},
    // '>' (62)
    {0x00,0x41,0x22,0x14,0x08},
    // '?' (63)
    {0x02,0x01,0x51,0x09,0x06},
    // '@' (64)
    {0x32,0x49,0x79,0x41,0x3E},
    // 'A' (65)
    {0x7E,0x11,0x11,0x11,0x7E},
    // 'B' (66)
    {0x7F,0x49,0x49,0x49,0x36},
    // 'C' (67)
    {0x3E,0x41,0x41,0x41,0x22},
    // 'D' (68)
    {0x7F,0x41,0x41,0x22,0x1C},
    // 'E' (69)
    {0x7F,0x49,0x49,0x49,0x41},
    // 'F' (70)
    {0x7F,0x09,0x09,0x09,0x01},
    // 'G' (71)
    {0x3E,0x41,0x49,0x49,0x7A},
    // 'H' (72)
    {0x7F,0x08,0x08,0x08,0x7F},
    // 'I' (73)
    {0x00,0x41,0x7F,0x41,0x00},
    // 'J' (74)
    {0x20,0x40,0x41,0x3F,0x01},
    // 'K' (75)
    {0x7F,0x08,0x14,0x22,0x41},
    // 'L' (76)
    {0x7F,0x40,0x40,0x40,0x40},
    // 'M' (77)
    {0x7F,0x02,0x0C,0x02,0x7F},
    // 'N' (78)
    {0x7F,0x04,0x08,0x10,0x7F},
    // 'O' (79)
    {0x3E,0x41,0x41,0x41,0x3E},
    // 'P' (80)
    {0x7F,0x09,0x09,0x09,0x06},
    // 'Q' (81)
    {0x3E,0x41,0x51,0x21,0x5E},
    // 'R' (82)
    {0x7F,0x09,0x19,0x29,0x46},
    // 'S' (83)
    {0x46,0x49,0x49,0x49,0x31},
    // 'T' (84)
    {0x01,0x01,0x7F,0x01,0x01},
    // 'U' (85)
    {0x3F,0x40,0x40,0x40,0x3F},
    // 'V' (86)
    {0x1F,0x20,0x40,0x20,0x1F},
    // 'W' (87)
    {0x3F,0x40,0x38,0x40,0x3F},
    // 'X' (88)
    {0x63,0x14,0x08,0x14,0x63},
    // 'Y' (89)
    {0x07,0x08,0x70,0x08,0x07},
    // 'Z' (90)
    {0x61,0x51,0x49,0x45,0x43},
};

// =============================================================================
// Fonctions de rendu texte pixel art
// =============================================================================

void render_char(SDL_Renderer *ren, int x, int y, char c,
                 SDL_Color col, int scale) {
    if (c < 32 || c > 90) c = '?';
    int idx = (unsigned char)c - 32;
    const unsigned char *glyph = FONT5x7[idx];
    SDL_SetRenderDrawColor(ren, col.r, col.g, col.b, col.a);
    for (int col_i = 0; col_i < 5; col_i++) {
        for (int row = 0; row < 7; row++) {
            if (glyph[col_i] & (1 << row)) {
                SDL_Rect px = { x + col_i * scale, y + row * scale, scale, scale };
                SDL_RenderFillRect(ren, &px);
            }
        }
    }
}

void render_string(SDL_Renderer *ren, int x, int y, const char *str,
                   SDL_Color col, int scale) {
    int cx = x;
    while (*str) {
        char c = *str;
        if (c >= 'a' && c <= 'z') c = c - 'a' + 'A'; // force uppercase
        render_char(ren, cx, y, c, col, scale);
        cx += (5 + 1) * scale; // 5px glyph + 1px espace
        str++;
    }
}

// =============================================================================
// Bloc 2.5D : face du dessus + ombre portee bas-droite
// =============================================================================
void render_block_25d(SDL_Renderer *ren,
                      int x_px, int y_px, int sz, int lift,
                      SDL_Color top_color, SDL_Color shadow_color) {
    // Ombre (face avant + face droite) : decalee de SHADOW_X/SHADOW_Y
    // et tronquee pour donner l'illusion d'un bloc en relief
    SDL_SetRenderDrawColor(ren, shadow_color.r, shadow_color.g,
                                shadow_color.b, shadow_color.a);

    // Face avant (bas du bloc) : rectangle fin en bas
    SDL_Rect front = {
        x_px + SHADOW_X,
        y_px + sz - lift,
        sz,
        SHADOW_Y + lift
    };
    SDL_RenderFillRect(ren, &front);

    // Face droite (cote du bloc) : rectangle fin a droite
    SDL_Rect right_face = {
        x_px + sz,
        y_px + SHADOW_Y - lift,
        SHADOW_X,
        sz
    };
    SDL_RenderFillRect(ren, &right_face);

    // Face du dessus : la vraie case
    SDL_SetRenderDrawColor(ren, top_color.r, top_color.g,
                                top_color.b, top_color.a);
    SDL_Rect top = { x_px, y_px - lift, sz, sz };
    SDL_RenderFillRect(ren, &top);
}

// =============================================================================
// Initialisation SDL2
// =============================================================================
int render_init(SDL_Window **win, SDL_Renderer **ren) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
        return -1;

    *win = SDL_CreateWindow(
        "ZONIX - Paper.io 2.5D",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W + SHADOW_X + 4,
        WINDOW_H + SHADOW_Y + 4 + 40,  // +40 pour le HUD en bas
        SDL_WINDOW_SHOWN
    );
    if (!*win) return -1;

    *ren = SDL_CreateRenderer(*win, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!*ren) return -1;

    SDL_SetRenderDrawBlendMode(*ren, SDL_BLENDMODE_NONE);
    return 0;
}

// =============================================================================
// Rendu principal 2.5D
// =============================================================================
void render_draw(SDL_Renderer *ren, GameState *gs, int my_id) {
    // Fond
    SDL_Color bg = COLOR_BG_TOP;
    SDL_SetRenderDrawColor(ren, bg.r, bg.g, bg.b, 255);
    SDL_RenderClear(ren);

    // ----- Cases du territoire -----
    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {
            int owner = gs->map[y][x];
            if (owner == 0) {
                // Case vide : dessin d'une grille subtile
                SDL_Color bgsh = COLOR_BG_SHADOW;
                render_block_25d(ren,
                    x * CELL_SIZE, y * CELL_SIZE,
                    CELL_SIZE - 1, 1,
                    COLOR_BG_TOP, bgsh);
            } else {
                // Case de territoire
                SDL_Color top = PLAYER_COLORS[owner-1].territory;
                SDL_Color shd = PLAYER_COLORS[owner-1].shadow;
                render_block_25d(ren,
                    x * CELL_SIZE, y * CELL_SIZE,
                    CELL_SIZE - 1, 2,
                    top, shd);
            }
        }
    }

    // ----- Trainées des joueurs -----
    for (int i = 0; i < gs->player_count; i++) {
        Player *p = &gs->players[i];
        if (!p->alive) continue;

        SDL_Color tc  = PLAYER_COLORS[i].trail;
        SDL_Color tsh = PLAYER_COLORS[i].trail_shadow;

        for (int t = 0; t < p->trail_len; t++) {
            int tx = p->trail[t][0];
            int ty = p->trail[t][1];
            render_block_25d(ren,
                tx * CELL_SIZE, ty * CELL_SIZE,
                CELL_SIZE - 1, 3,  // trainee plus haute que le territoire
                tc, tsh);
        }
    }

    // ----- Têtes des joueurs (les plus hautes -> dessinees en dernier) -----
    for (int i = 0; i < gs->player_count; i++) {
        Player *p = &gs->players[i];
        if (!p->alive) continue;

        SDL_Color hc  = PLAYER_COLORS[i].head;
        SDL_Color hsh = PLAYER_COLORS[i].head_shadow;

        // Tete plus grande et plus elevee pour se distinguer
        int extra = (i + 1 == my_id) ? 1 : 0;  // joueur local encore plus visible
        int lift  = HEAD_LIFT + 3 + extra;
        int sz    = CELL_SIZE - 1 + extra * 2;
        int ox    = p->x * CELL_SIZE - extra;
        int oy    = p->y * CELL_SIZE - extra;

        render_block_25d(ren, ox, oy, sz, lift, hc, hsh);

        // Indicateur "vous" : petit carre blanc au centre de la tete
        if (i + 1 == my_id) {
            SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
            SDL_Rect dot = {
                ox + sz/2 - 1,
                oy - lift + sz/2 - 1,
                2, 2
            };
            SDL_RenderFillRect(ren, &dot);
        }
    }

    // ----- HUD -----
    render_hud(ren, gs, my_id);

    SDL_RenderPresent(ren);
}

// =============================================================================
// HUD : scores en bas de l'ecran
// =============================================================================
void render_hud(SDL_Renderer *ren, GameState *gs, int my_id) {
    int hud_y = WINDOW_H + SHADOW_Y + 6;
    int total  = GRID_W * GRID_H;

    // Fond HUD
    SDL_SetRenderDrawColor(ren, 12, 12, 20, 255);
    SDL_Rect hud_bg = { 0, WINDOW_H + SHADOW_Y, WINDOW_W + SHADOW_X + 4, 40 };
    SDL_RenderFillRect(ren, &hud_bg);

    // Affichage de chaque joueur
    for (int i = 0; i < gs->player_count; i++) {
        Player *p = &gs->players[i];
        int x_offset = i * (WINDOW_W / gs->player_count) + 4;

        SDL_Color col = PLAYER_COLORS[i].territory;

        // Indicateur de couleur (petit carre)
        if (i + 1 == my_id) {
            // Surligne le joueur local avec un contour
            SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
            SDL_Rect outline = { x_offset - 1, hud_y - 1, 10, 10 };
            SDL_RenderFillRect(ren, &outline);
        }
        SDL_SetRenderDrawColor(ren, col.r, col.g, col.b, 255);
        SDL_Rect sq = { x_offset, hud_y, 8, 8 };
        SDL_RenderFillRect(ren, &sq);

        // "P1" ou "P2" etc
        char label[4] = "P?";
        label[1] = '1' + i;
        SDL_Color text_col = p->alive ? col : (SDL_Color){80, 80, 80, 255};
        render_string(ren, x_offset + 12, hud_y, label, text_col, 1);

        // Pourcentage de territoire
        if (p->alive) {
            int pct = (total > 0) ? (p->score * 100 / total) : 0;
            char num[8];
            snprintf(num, sizeof(num), "%d%%", pct);
            render_string(ren, x_offset + 12, hud_y + 10, num, text_col, 1);
        } else {
            render_string(ren, x_offset + 12, hud_y + 10, "MORT",
                          (SDL_Color){80,80,80,255}, 1);
        }
    }
}

// =============================================================================
// Ecran de fin de jeu
// =============================================================================
void render_game_over(SDL_Renderer *ren, int winner_id, GameState *gs) {
    // Fond semi-transparent noir
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 180);
    SDL_Rect overlay = { 0, 0, WINDOW_W + SHADOW_X + 4, WINDOW_H + SHADOW_Y + 44 };
    SDL_RenderFillRect(ren, &overlay);
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_NONE);

    int cx = (WINDOW_W / 2) - 80;
    int cy = (WINDOW_H / 2) - 30;

    // "GAME OVER"
    render_string(ren, cx, cy, "GAME OVER",
                  (SDL_Color){255, 255, 255, 255}, 2);

    // "JOUEUR X GAGNE"
    if (winner_id >= 1 && winner_id <= MAX_PLAYERS) {
        SDL_Color wcol = PLAYER_COLORS[winner_id - 1].territory;
        char msg[32] = "JOUEUR ? GAGNE";
        msg[7] = '0' + winner_id;
        render_string(ren, cx - 10, cy + 24, msg, wcol, 2);
    } else {
        render_string(ren, cx, cy + 24, "EGALITE",
                      (SDL_Color){200, 200, 200, 255}, 2);
    }

    // Scores finaux
    int sy = cy + 55;
    for (int i = 0; i < gs->player_count; i++) {
        int pct = (GRID_W * GRID_H > 0)
                  ? (gs->players[i].score * 100 / (GRID_W * GRID_H)) : 0;
        SDL_Color c = PLAYER_COLORS[i].territory;
        char line[32];
        snprintf(line, sizeof(line), "P%d  %d%%", i+1, pct);
        render_string(ren, cx + i * 90, sy, line, c, 2);
    }

    // "APPUIE SUR ESPACE"
    render_string(ren, cx - 40, cy + 85, "APPUIE SUR ESPACE",
                  (SDL_Color){180, 180, 180, 255}, 1);

    SDL_RenderPresent(ren);
}

void render_cleanup(SDL_Window *win, SDL_Renderer *ren) {
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
