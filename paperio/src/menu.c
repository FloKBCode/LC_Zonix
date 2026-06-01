// menu.c - tous les ecrans menu en SDL2 pur (pas de SDL2_ttf)
#include <SDL2/SDL.h>
#include <string.h>
#include <stdio.h>
#include "../include/menu.h"
#include "../include/render.h"
#include "../include/constants.h"
#include "../include/colors.h"

// =============================================================================
// Utilitaires internes
// =============================================================================

// Largeur en pixels d'une chaine (police 5x7, scale pixels par case)
static int str_width(const char *s, int scale) {
    return (int)strlen(s) * 6 * scale;
}

// Dessine un bouton rectangulaire avec label centre
// Retourne 1 si la souris est dedans (hover)
static int draw_button(SDL_Renderer *ren, SDL_Rect rect, const char *label,
                        SDL_Color bg, SDL_Color hover_bg, SDL_Color text_col,
                        int mx, int my) {
    int hovered = (mx >= rect.x && mx < rect.x + rect.w &&
                   my >= rect.y && my < rect.y + rect.h);

    SDL_Color use_bg = hovered ? hover_bg : bg;
    SDL_SetRenderDrawColor(ren, use_bg.r, use_bg.g, use_bg.b, 255);
    SDL_RenderFillRect(ren, &rect);

    // Contour blanc si survole
    if (hovered) {
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderDrawRect(ren, &rect);
    }

    // Label centre
    int tw = str_width(label, 2);
    int tx = rect.x + (rect.w - tw) / 2;
    int ty = rect.y + (rect.h - 14) / 2;
    render_string(ren, tx, ty, label, text_col, 2);

    return hovered;
}

// Fond degrade vertical
static void draw_background(SDL_Renderer *ren) {
    for (int y = 0; y < WINDOW_H + 44; y++) {
        int r = 10 + y * 6  / (WINDOW_H + 44);
        int g = 10 + y * 4  / (WINDOW_H + 44);
        int b = 20 + y * 20 / (WINDOW_H + 44);
        SDL_SetRenderDrawColor(ren, r, g, b, 255);
        SDL_RenderDrawLine(ren, 0, y, WINDOW_W + 4, y);
    }
}

// Titre "ZONIX" en gros pixel art colore
static void draw_title(SDL_Renderer *ren) {
    const char *title = "ZONIX";
    int tw = str_width(title, 4);
    int tx = (WINDOW_W - tw) / 2;

    // Ombre du titre
    render_string(ren, tx + 3, 48 + 3, title, (SDL_Color){0, 0, 0, 255}, 4);

    // Titre colore lettre par lettre (degrade arc-en-ciel)
    SDL_Color rainbow[5] = {
        {255,  60,  60, 255},
        {255, 165,   0, 255},
        {255, 255,  60, 255},
        { 60, 200,  60, 255},
        { 60, 140, 255, 255},
    };
    char letter[2] = {0, 0};
    for (int i = 0; i < 5; i++) {
        letter[0] = title[i];
        render_string(ren, tx + i * 6 * 4, 48, letter, rainbow[i], 4);
    }

    // Sous-titre
    render_string(ren, (WINDOW_W - str_width("PAPER.IO 2.5D MULTIJOUEUR", 1)) / 2,
                  100, "PAPER.IO 2.5D MULTIJOUEUR",
                  (SDL_Color){160, 160, 200, 255}, 1);
}

// =============================================================================
// Menu principal
// =============================================================================
MenuResult menu_main(SDL_Renderer *ren) {
    SDL_Event e;
    int mx = 0, my = 0;

    SDL_Rect btn_server  = { WINDOW_W/2 - 120, 150, 240, 40 };
    SDL_Rect btn_client  = { WINDOW_W/2 - 120, 205, 240, 40 };
    SDL_Rect btn_tuto    = { WINDOW_W/2 - 120, 260, 240, 40 };
    SDL_Rect btn_quit    = { WINDOW_W/2 - 120, 315, 240, 40 };

    SDL_Color bg_btn   = { 30,  35,  55, 255 };
    SDL_Color bg_hover = { 60,  70, 110, 255 };
    SDL_Color text_col = {230, 230, 230, 255 };

    while (1) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                return MENU_RESULT_QUIT;

            if (e.type == SDL_MOUSEMOTION) {
                mx = e.motion.x;
                my = e.motion.y;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                mx = e.button.x;
                my = e.button.y;
                if (mx >= btn_server.x && mx < btn_server.x + btn_server.w &&
                    my >= btn_server.y && my < btn_server.y + btn_server.h)
                    return MENU_RESULT_SERVER;
                if (mx >= btn_client.x && mx < btn_client.x + btn_client.w &&
                    my >= btn_client.y && my < btn_client.y + btn_client.h)
                    return MENU_RESULT_CLIENT;
                if (mx >= btn_tuto.x && mx < btn_tuto.x + btn_tuto.w &&
                    my >= btn_tuto.y && my < btn_tuto.y + btn_tuto.h) {
                    menu_tutorial(ren);
                }
                if (mx >= btn_quit.x && mx < btn_quit.x + btn_quit.w &&
                    my >= btn_quit.y && my < btn_quit.y + btn_quit.h)
                    return MENU_RESULT_QUIT;
            }
        }

        draw_background(ren);
        draw_title(ren);

        draw_button(ren, btn_server, "HEBERGER", bg_btn, bg_hover, text_col, mx, my);
        draw_button(ren, btn_client, "REJOINDRE", bg_btn, bg_hover, text_col, mx, my);
        draw_button(ren, btn_tuto,   "TUTORIEL", bg_btn, bg_hover, text_col, mx, my);
        draw_button(ren, btn_quit,   "QUITTER",
                    (SDL_Color){50,20,20,255}, (SDL_Color){100,30,30,255},
                    text_col, mx, my);

        // Version
        render_string(ren, 4, WINDOW_H - 12, "V1.0 - YNOV B1",
                      (SDL_Color){60, 60, 90, 255}, 1);

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }
}

// =============================================================================
// Menu configuration réseau
// =============================================================================
MenuResult menu_network(SDL_Renderer *ren, NetworkConfig *cfg, int is_server) {
    SDL_Event e;
    int mx = 0, my = 0;

    // Valeurs par defaut
    if (cfg->port == 0) cfg->port = SERVER_PORT;
    if (!is_server && cfg->ip[0] == '\0')
        strncpy(cfg->ip, "192.168.1.", 63);
    if (is_server && cfg->player_count == 0)
        cfg->player_count = 2;

    // Champ actif : 0=IP, 1=PORT, 2=NB_JOUEURS
    int active_field = is_server ? 2 : 0;

    // Buffers d'edition
    char buf_ip[64];
    char buf_port[8];
    char buf_nb[4];

    strncpy(buf_ip,   cfg->ip,                   63);
    snprintf(buf_port, sizeof(buf_port), "%d",   cfg->port);
    snprintf(buf_nb,   sizeof(buf_nb),   "%d",   cfg->player_count);

    SDL_StartTextInput();

    SDL_Rect field_ip   = { WINDOW_W/2 - 150, 180, 300, 32 };
    SDL_Rect field_port = { WINDOW_W/2 - 150, 240, 300, 32 };
    SDL_Rect field_nb   = { WINDOW_W/2 - 150, 300, 300, 32 };
    SDL_Rect btn_ok     = { WINDOW_W/2 - 80,  360, 160, 40 };
    SDL_Rect btn_back   = { WINDOW_W/2 - 80,  410, 160, 35 };

    while (1) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                SDL_StopTextInput();
                return MENU_RESULT_QUIT;
            }
            if (e.type == SDL_MOUSEMOTION) {
                mx = e.motion.x; my = e.motion.y;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                mx = e.button.x; my = e.button.y;
                // Sélection champ
                if (!is_server &&
                    mx >= field_ip.x && mx < field_ip.x+field_ip.w &&
                    my >= field_ip.y && my < field_ip.y+field_ip.h)   active_field = 0;
                if (mx >= field_port.x && mx < field_port.x+field_port.w &&
                    my >= field_port.y && my < field_port.y+field_port.h) active_field = 1;
                if (is_server &&
                    mx >= field_nb.x && mx < field_nb.x+field_nb.w &&
                    my >= field_nb.y && my < field_nb.y+field_nb.h)   active_field = 2;

                // Bouton OK
                if (mx >= btn_ok.x && mx < btn_ok.x+btn_ok.w &&
                    my >= btn_ok.y && my < btn_ok.y+btn_ok.h) {
                    strncpy(cfg->ip,    buf_ip,   63);
                    cfg->port         = atoi(buf_port);
                    cfg->player_count = atoi(buf_nb);
                    if (cfg->port <= 0)         cfg->port = SERVER_PORT;
                    if (cfg->player_count < 2)  cfg->player_count = 2;
                    if (cfg->player_count > 4)  cfg->player_count = 4;
                    SDL_StopTextInput();
                    return is_server ? MENU_RESULT_SERVER : MENU_RESULT_CLIENT;
                }
                // Bouton Retour
                if (mx >= btn_back.x && mx < btn_back.x+btn_back.w &&
                    my >= btn_back.y && my < btn_back.y+btn_back.h) {
                    SDL_StopTextInput();
                    return MENU_RESULT_BACK;
                }
            }
            if (e.type == SDL_TEXTINPUT) {
                char *target = NULL;
                int max_len = 63;
                if      (active_field == 0 && !is_server) { target = buf_ip;   max_len = 63; }
                else if (active_field == 1)               { target = buf_port; max_len = 5;  }
                else if (active_field == 2 && is_server)  { target = buf_nb;   max_len = 1;  }
                if (target && (int)strlen(target) < max_len)
                    strncat(target, e.text.text, max_len - strlen(target));
            }
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_BACKSPACE) {
                    char *target = NULL;
                    if      (active_field == 0 && !is_server) target = buf_ip;
                    else if (active_field == 1)               target = buf_port;
                    else if (active_field == 2 && is_server)  target = buf_nb;
                    if (target && strlen(target) > 0)
                        target[strlen(target)-1] = '\0';
                }
                if (e.key.keysym.sym == SDLK_TAB)
                    active_field = (active_field + 1) % 3;
                if (e.key.keysym.sym == SDLK_RETURN) {
                    strncpy(cfg->ip,    buf_ip,   63);
                    cfg->port         = atoi(buf_port);
                    cfg->player_count = atoi(buf_nb);
                    if (cfg->port <= 0)        cfg->port = SERVER_PORT;
                    if (cfg->player_count < 2) cfg->player_count = 2;
                    if (cfg->player_count > 4) cfg->player_count = 4;
                    SDL_StopTextInput();
                    return is_server ? MENU_RESULT_SERVER : MENU_RESULT_CLIENT;
                }
            }
        }

        draw_background(ren);
        draw_title(ren);

        const char *page_title = is_server ? "HEBERGER UNE PARTIE" : "REJOINDRE UNE PARTIE";
        render_string(ren,
            (WINDOW_W - str_width(page_title, 2)) / 2,
            130, page_title, (SDL_Color){200,200,255,255}, 2);

        // Champ IP (seulement pour le client)
        if (!is_server) {
            render_string(ren, WINDOW_W/2 - 150, 162, "ADRESSE IP DU SERVEUR",
                          (SDL_Color){160,160,200,255}, 1);
            SDL_Color fc = (active_field == 0)
                           ? (SDL_Color){60,80,130,255}
                           : (SDL_Color){30,35,55,255};
            SDL_SetRenderDrawColor(ren, fc.r, fc.g, fc.b, 255);
            SDL_RenderFillRect(ren, &field_ip);
            if (active_field == 0) {
                SDL_SetRenderDrawColor(ren, 100, 140, 220, 255);
                SDL_RenderDrawRect(ren, &field_ip);
            }
            render_string(ren, field_ip.x + 6, field_ip.y + 10,
                          buf_ip, (SDL_Color){230,230,230,255}, 1);
        }

        // Champ PORT
        render_string(ren, WINDOW_W/2 - 150, 222, "PORT (DEFAUT 5555)",
                      (SDL_Color){160,160,200,255}, 1);
        SDL_Color fc_p = (active_field == 1)
                         ? (SDL_Color){60,80,130,255}
                         : (SDL_Color){30,35,55,255};
        SDL_SetRenderDrawColor(ren, fc_p.r, fc_p.g, fc_p.b, 255);
        SDL_RenderFillRect(ren, &field_port);
        if (active_field == 1) {
            SDL_SetRenderDrawColor(ren, 100, 140, 220, 255);
            SDL_RenderDrawRect(ren, &field_port);
        }
        render_string(ren, field_port.x + 6, field_port.y + 10,
                      buf_port, (SDL_Color){230,230,230,255}, 1);

        // Champ NB JOUEURS (seulement pour le serveur)
        if (is_server) {
            render_string(ren, WINDOW_W/2 - 150, 282, "NOMBRE DE JOUEURS (2-4)",
                          (SDL_Color){160,160,200,255}, 1);
            SDL_Color fc_n = (active_field == 2)
                             ? (SDL_Color){60,80,130,255}
                             : (SDL_Color){30,35,55,255};
            SDL_SetRenderDrawColor(ren, fc_n.r, fc_n.g, fc_n.b, 255);
            SDL_RenderFillRect(ren, &field_nb);
            if (active_field == 2) {
                SDL_SetRenderDrawColor(ren, 100, 140, 220, 255);
                SDL_RenderDrawRect(ren, &field_nb);
            }
            render_string(ren, field_nb.x + 6, field_nb.y + 10,
                          buf_nb, (SDL_Color){230,230,230,255}, 1);
        }

        draw_button(ren, btn_ok, "LANCER",
                    (SDL_Color){30,80,30,255}, (SDL_Color){50,140,50,255},
                    (SDL_Color){230,255,230,255}, mx, my);
        draw_button(ren, btn_back, "RETOUR",
                    (SDL_Color){40,35,55,255}, (SDL_Color){70,65,95,255},
                    (SDL_Color){200,200,230,255}, mx, my);

        render_string(ren, WINDOW_W/2 - 100, 455,
                      "TAB = CHAMP SUIVANT  ENTREE = OK",
                      (SDL_Color){80,80,110,255}, 1);

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }
}

// =============================================================================
// Tutoriel
// =============================================================================
void menu_tutorial(SDL_Renderer *ren) {
    SDL_Event e;

    typedef struct { const char *line; SDL_Color col; } TutoLine;
    TutoLine lines[] = {
        { "TUTORIEL",                  {255, 255, 100, 255} },
        { "",                          {0,0,0,0} },
        { "OBJECTIF",                  {100, 220, 255, 255} },
        { "CONQUIERE LE PLUS GRAND",   {200, 200, 200, 255} },
        { "TERRITOIRE POSSIBLE.",      {200, 200, 200, 255} },
        { "",                          {0,0,0,0} },
        { "CONTROLES",                 {100, 220, 255, 255} },
        { "Z / FLECHE HAUT  = HAUT",   {200, 200, 200, 255} },
        { "S / FLECHE BAS   = BAS",    {200, 200, 200, 255} },
        { "Q / FLECHE GAUCHE= GAUCHE", {200, 200, 200, 255} },
        { "D / FLECHE DROITE= DROITE", {200, 200, 200, 255} },
        { "",                          {0,0,0,0} },
        { "REGLES",                    {100, 220, 255, 255} },
        { "SORS DE TON TERRITOIRE",    {200, 200, 200, 255} },
        { "POUR TRACER UNE TRAINEE.",  {200, 200, 200, 255} },
        { "REVIENS SUR TON TERRAIN",   {200, 200, 200, 255} },
        { "POUR CAPTURER LA ZONE.",    {200, 200, 200, 255} },
        { "",                          {0,0,0,0} },
        { "MOURIR SI...",              {255, 120, 120, 255} },
        { "- TU TOUCHES TON TRAIL",    {200, 200, 200, 255} },
        { "- TU TOUCHES UN BORD",      {200, 200, 200, 255} },
        { "- TU CROISES UN ENNEMI",    {200, 200, 200, 255} },
        { "",                          {0,0,0,0} },
        { "COUPE LE TRAIL ENNEMI",     {120, 255, 120, 255} },
        { "POUR LE TUER DIRECTEMENT!", {120, 255, 120, 255} },
        { "",                          {0,0,0,0} },
        { "APPUIE SUR UNE TOUCHE",     {160, 160, 160, 255} },
    };

    int n = (int)(sizeof(lines) / sizeof(lines[0]));

    while (1) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)         return;
            if (e.type == SDL_KEYDOWN)      return;
            if (e.type == SDL_MOUSEBUTTONDOWN) return;
        }

        draw_background(ren);

        int start_y = 30;
        for (int i = 0; i < n; i++) {
            if (lines[i].line[0] == '\0') continue;
            int scale = (i == 0) ? 2 : 1;
            int lw = (int)strlen(lines[i].line) * 6 * scale;
            render_string(ren,
                (WINDOW_W - lw) / 2,
                start_y + i * (scale == 2 ? 20 : 12),
                lines[i].line, lines[i].col, scale);
        }

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }
}

// =============================================================================
// Ecran "en attente de joueurs"
// =============================================================================
void menu_waiting(SDL_Renderer *ren, int connected, int expected) {
    draw_background(ren);
    draw_title(ren);

    render_string(ren,
        (WINDOW_W - str_width("EN ATTENTE DE JOUEURS...", 2)) / 2,
        180, "EN ATTENTE DE JOUEURS...",
        (SDL_Color){200, 200, 100, 255}, 2);

    char buf[64];
    snprintf(buf, sizeof(buf), "%d / %d CONNECTES", connected, expected);
    render_string(ren,
        (WINDOW_W - str_width(buf, 2)) / 2,
        220, buf,
        (SDL_Color){150, 220, 150, 255}, 2);

    // Barre de progression simple
    int bar_w = 300;
    int bar_x = (WINDOW_W - bar_w) / 2;
    int bar_y = 260;
    SDL_SetRenderDrawColor(ren, 40, 40, 60, 255);
    SDL_Rect bar_bg = { bar_x, bar_y, bar_w, 16 };
    SDL_RenderFillRect(ren, &bar_bg);
    int fill = (expected > 0) ? (bar_w * connected / expected) : 0;
    SDL_SetRenderDrawColor(ren, 80, 200, 80, 255);
    SDL_Rect bar_fill = { bar_x, bar_y, fill, 16 };
    SDL_RenderFillRect(ren, &bar_fill);

    render_string(ren,
        (WINDOW_W - str_width("LA PARTIE COMMENCE DES QUE", 1)) / 2,
        295, "LA PARTIE COMMENCE DES QUE",
        (SDL_Color){120, 120, 160, 255}, 1);
    render_string(ren,
        (WINDOW_W - str_width("TOUS LES JOUEURS SONT LA.", 1)) / 2,
        307, "TOUS LES JOUEURS SONT LA.",
        (SDL_Color){120, 120, 160, 255}, 1);

    SDL_RenderPresent(ren);
}

// =============================================================================
// Ecran "connexion en cours"
// =============================================================================
void menu_connecting(SDL_Renderer *ren, const char *ip, int port) {
    draw_background(ren);
    draw_title(ren);

    render_string(ren,
        (WINDOW_W - str_width("CONNEXION EN COURS...", 2)) / 2,
        180, "CONNEXION EN COURS...",
        (SDL_Color){200, 200, 100, 255}, 2);

    char buf[80];
    snprintf(buf, sizeof(buf), "SERVEUR : %s PORT %d", ip, port);
    // Convertit en majuscules (ip peut avoir des minuscules)
    for (int i = 0; buf[i]; i++)
        if (buf[i] >= 'a' && buf[i] <= 'z') buf[i] -= 32;

    render_string(ren,
        (WINDOW_W - str_width(buf, 1)) / 2,
        220, buf,
        (SDL_Color){160, 160, 200, 255}, 1);

    SDL_RenderPresent(ren);
}
