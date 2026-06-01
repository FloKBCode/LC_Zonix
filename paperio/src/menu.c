#include <SDL2/SDL.h>
#include <string.h>
#include <stdio.h>
#include "../include/menu.h"
#include "../include/render.h"
#include "../include/colors.h"
#include "../include/constants.h"

#define ROS_WHITE  ((SDL_Color){255,255,255,255})
#define ROS_LTGRAY ((SDL_Color){192,192,192,255})
#define ROS_GRAY   ((SDL_Color){176,183,196,255})
#define ROS_DKGRAY ((SDL_Color){128,128,128,255})
#define ROS_BLACK  ((SDL_Color){0,0,0,255})
#define ROS_NAVY   ((SDL_Color){0,0,128,255})
#define ROS_ACCENT ((SDL_Color){0,119,255,255})
#define ROS_ALERT  ((SDL_Color){220,53,69,255})
#define ROS_PAPER  ((SDL_Color){245,245,240,255})

static void ros_fill(SDL_Renderer *ren, SDL_Rect r, SDL_Color c) {
    SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, 255);
    SDL_RenderFillRect(ren, &r);
}

static void ros_bevel(SDL_Renderer *ren, SDL_Rect r, int raised) {
    SDL_Color hi  = raised ? ROS_WHITE  : ROS_DKGRAY;
    SDL_Color lo  = raised ? ROS_DKGRAY : ROS_WHITE;
    SDL_Color hi2 = raised ? ROS_LTGRAY : (SDL_Color){80,80,80,255};
    SDL_Color lo2 = raised ? (SDL_Color){80,80,80,255} : ROS_LTGRAY;
    ros_fill(ren, r, ROS_LTGRAY);
    SDL_SetRenderDrawColor(ren, hi.r, hi.g, hi.b, 255);
    SDL_RenderDrawLine(ren, r.x, r.y, r.x+r.w-1, r.y);
    SDL_RenderDrawLine(ren, r.x, r.y, r.x, r.y+r.h-1);
    SDL_SetRenderDrawColor(ren, lo2.r, lo2.g, lo2.b, 255);
    SDL_RenderDrawLine(ren, r.x+r.w-1, r.y, r.x+r.w-1, r.y+r.h-1);
    SDL_RenderDrawLine(ren, r.x, r.y+r.h-1, r.x+r.w-1, r.y+r.h-1);
    SDL_SetRenderDrawColor(ren, hi2.r, hi2.g, hi2.b, 255);
    SDL_RenderDrawLine(ren, r.x+1, r.y+1, r.x+r.w-2, r.y+1);
    SDL_RenderDrawLine(ren, r.x+1, r.y+1, r.x+1, r.y+r.h-2);
    SDL_SetRenderDrawColor(ren, lo.r, lo.g, lo.b, 255);
    SDL_RenderDrawLine(ren, r.x+r.w-2, r.y+1, r.x+r.w-2, r.y+r.h-2);
    SDL_RenderDrawLine(ren, r.x+1, r.y+r.h-2, r.x+r.w-2, r.y+r.h-2);
}

static void ros_window(SDL_Renderer *ren, SDL_Rect r, const char *title, int active) {
    ros_bevel(ren, r, 1);
    SDL_Rect tb = {r.x+3, r.y+3, r.w-6, 18};
    SDL_Color tbc = active ? ROS_NAVY : ROS_DKGRAY;
    SDL_SetRenderDrawColor(ren, tbc.r, tbc.g, tbc.b, 255);
    SDL_RenderFillRect(ren, &tb);
    SDL_SetRenderDrawColor(ren, active?30:90, active?30:90, active?160:130, 255);
    SDL_RenderDrawLine(ren, tb.x+1, tb.y+1, tb.x+tb.w-2, tb.y+1);
    render_string(ren, r.x+7, r.y+6, title, ROS_WHITE, 1);
    SDL_Rect xb = {r.x+r.w-20, r.y+4, 16, 14};
    ros_bevel(ren, xb, 1);
    render_string(ren, xb.x+4, xb.y+3, "X", ROS_BLACK, 1);
    SDL_Rect mb = {r.x+r.w-38, r.y+4, 16, 14};
    ros_bevel(ren, mb, 1);
    render_string(ren, mb.x+4, mb.y+3, "-", ROS_BLACK, 1);
}

static int ros_button_ex(SDL_Renderer *ren, SDL_Rect r, const char *label,
                          int mx, int my, int pressed) {
    int hov = (mx>=r.x&&mx<r.x+r.w&&my>=r.y&&my<r.y+r.h);
    if (hov && !pressed) {
        SDL_SetRenderDrawColor(ren, 210,215,225,255);
        SDL_RenderFillRect(ren, &r);
    }
    ros_bevel(ren, r, !pressed);
    int tw=text_w(label, 1), tx=r.x+(r.w-tw)/2, ty=r.y+(r.h-7)/2+(pressed?1:0);
    render_string(ren, tx, ty, label, ROS_BLACK, 1);
    return hov;
}

static void ros_input_field(SDL_Renderer *ren, SDL_Rect r, const char *val, int active) {
    ros_fill(ren, r, ROS_PAPER);
    ros_bevel(ren, r, 0);
    if (active) {
        SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(ren, 0,119,255,40);
        SDL_Rect in={r.x+2,r.y+2,r.w-4,r.h-4};
        SDL_RenderFillRect(ren, &in);
        SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_NONE);
    }
    render_string(ren, r.x+4, r.y+(r.h-7)/2, val, ROS_BLACK, 1);
    if (active && (SDL_GetTicks()/400)%2==0) {
        int cx=r.x+4+text_w(val, 1);
        SDL_SetRenderDrawColor(ren,0,0,0,255);
        SDL_RenderDrawLine(ren,cx,r.y+3,cx,r.y+r.h-4);
    }
}

static void ros_label_sep(SDL_Renderer *ren, int x, int y, const char *txt) {
    render_string(ren, x, y, txt, ROS_DKGRAY, 1);
    int w=text_w(txt, 1)+40;
    SDL_SetRenderDrawColor(ren,ROS_DKGRAY.r,ROS_DKGRAY.g,ROS_DKGRAY.b,255);
    SDL_RenderDrawLine(ren,x,y+9,x+w,y+9);
    SDL_SetRenderDrawColor(ren,ROS_WHITE.r,ROS_WHITE.g,ROS_WHITE.b,255);
    SDL_RenderDrawLine(ren,x,y+10,x+w,y+10);
}

static void draw_desktop(SDL_Renderer *ren) {
    SDL_SetRenderDrawColor(ren, 0, 128, 128, 255);
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren, 0, 112, 112, 255);
    for (int y=0;y<WINDOW_H+44;y+=4)
        for (int x=0;x<WINDOW_W+8;x+=4)
            SDL_RenderDrawPoint(ren,x,y);
    SDL_Rect taskbar={0,WINDOW_H+SHADOW_Y+SHADOW_X,WINDOW_W+8,28};
    ros_bevel(ren,taskbar,1);
    SDL_Rect start={2,taskbar.y+2,70,22};
    ros_bevel(ren,start,1);
    render_string(ren,start.x+4,start.y+7,"DEMARRER",ROS_BLACK,1);
    Uint32 t=SDL_GetTicks()/1000;
    char clock_str[8]; snprintf(clock_str,8,"%02d:%02d",(int)(t/60)%60,(int)(t%60));
    render_string(ren,WINDOW_W-text_w(clock_str,1)-8,taskbar.y+8,clock_str,ROS_BLACK,1);
}

// Etat muet global
static int g_muted = 0;
int audio_is_muted(void) { return g_muted; }

MenuResult menu_main(SDL_Renderer *ren) {
    SDL_Event e;
    int mx=0, my=0, sel=0;

    SDL_Rect win      = {WINDOW_W/2-160, WINDOW_H/2-120, 320, 240};
    SDL_Rect btn_client = {win.x+20, win.y+50,  280, 32};
    SDL_Rect btn_tuto   = {win.x+20, win.y+96,  280, 32};
    SDL_Rect btn_quit   = {win.x+20, win.y+148, 280, 32};
    SDL_Rect btn_mute   = {win.x+win.w-50, win.y+4, 44, 16};

    while (1) {
        while (SDL_PollEvent(&e)) {
            if (e.type==SDL_QUIT) return MENU_RESULT_QUIT;
            if (e.type==SDL_MOUSEMOTION){mx=e.motion.x;my=e.motion.y;}
            if (e.type==SDL_KEYDOWN) {
                if (e.key.keysym.sym==SDLK_ESCAPE) return MENU_RESULT_QUIT;
                if (e.key.keysym.sym==SDLK_UP||e.key.keysym.sym==SDLK_z)
                    sel=(sel-1+3)%3;
                if (e.key.keysym.sym==SDLK_DOWN||e.key.keysym.sym==SDLK_s)
                    sel=(sel+1)%3;
                if (e.key.keysym.sym==SDLK_RETURN||e.key.keysym.sym==SDLK_KP_ENTER) {
                    if(sel==0) return MENU_RESULT_CLIENT;
                    if(sel==1) { menu_tutorial(ren); }
                    if(sel==2) return MENU_RESULT_QUIT;
                }
            }
            if (e.type==SDL_MOUSEBUTTONDOWN&&e.button.button==SDL_BUTTON_LEFT){
                mx=e.button.x; my=e.button.y;
                if(mx>=btn_client.x&&mx<btn_client.x+btn_client.w&&my>=btn_client.y&&my<btn_client.y+btn_client.h) return MENU_RESULT_CLIENT;
                if(mx>=btn_tuto.x&&mx<btn_tuto.x+btn_tuto.w&&my>=btn_tuto.y&&my<btn_tuto.y+btn_tuto.h) { menu_tutorial(ren); }
                if(mx>=btn_quit.x&&mx<btn_quit.x+btn_quit.w&&my>=btn_quit.y&&my<btn_quit.y+btn_quit.h) return MENU_RESULT_QUIT;
                if(mx>=btn_mute.x&&mx<btn_mute.x+btn_mute.w&&my>=btn_mute.y&&my<btn_mute.y+btn_mute.h) g_muted=!g_muted;
            }
        }

        draw_desktop(ren);
        ros_window(ren, win, "ZONIX.EXE", 1);

        // Logo
        render_string(ren, win.x+(win.w-text_w("ZONIX",2))/2, win.y+25, "ZONIX", ROS_ACCENT, 2);

        // Bouton MUET (vert=son actif, rouge=muet)
        {
            SDL_Color mute_bg = g_muted ? (SDL_Color){220,53,69,255} : (SDL_Color){40,120,40,255};
            SDL_SetRenderDrawColor(ren,mute_bg.r,mute_bg.g,mute_bg.b,255);
            SDL_RenderFillRect(ren,&btn_mute);
            ros_bevel(ren,btn_mute,1);
            SDL_SetRenderDrawColor(ren,mute_bg.r,mute_bg.g,mute_bg.b,255);
            SDL_Rect mf={btn_mute.x+2,btn_mute.y+2,btn_mute.w-4,btn_mute.h-4};
            SDL_RenderFillRect(ren,&mf);
            const char *mlbl = g_muted ? "MUET" : "SON";
            render_string(ren,btn_mute.x+(btn_mute.w-text_w(mlbl,1))/2,btn_mute.y+4,mlbl,ROS_WHITE,1);
        }

        // Boutons principaux avec surlignage clavier
        ros_button_ex(ren, btn_client, "REJOINDRE UNE PARTIE", sel==0?btn_client.x+1:mx, sel==0?btn_client.y+1:my, 0);
        ros_button_ex(ren, btn_tuto,   "TUTORIEL",              sel==1?btn_tuto.x+1:mx,   sel==1?btn_tuto.y+1:my,   0);

        // Bouton QUITTER rouge
        ros_bevel(ren, btn_quit, 1);
        SDL_SetRenderDrawColor(ren,220,53,69,255);
        SDL_Rect qfill={btn_quit.x+2,btn_quit.y+2,btn_quit.w-4,btn_quit.h-4};
        SDL_RenderFillRect(ren,&qfill);
        if(sel==2){
            SDL_SetRenderDrawColor(ren,255,100,100,255);
            SDL_RenderDrawRect(ren,&btn_quit);
        }
        render_string(ren,btn_quit.x+(btn_quit.w-text_w("QUITTER",1))/2,btn_quit.y+11,"QUITTER",ROS_WHITE,1);

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }
}

MenuResult menu_network(SDL_Renderer *ren, NetworkConfig *cfg, int is_server) {
    SDL_Event e;
    int mx=0,my=0,active=is_server?1:0;

    if (!cfg->port) cfg->port=SERVER_PORT;
    if (!is_server && !cfg->ip[0]) strncpy(cfg->ip,"192.168.1.",63);
    if (is_server && !cfg->player_count) cfg->player_count=2;

    char buf_ip[64], buf_port[8], buf_nb[4];
    strncpy(buf_ip,cfg->ip,63); buf_ip[63]=0;
    snprintf(buf_port,8,"%d",cfg->port);
    snprintf(buf_nb,4,"%d",cfg->player_count);

    SDL_StartTextInput();

    SDL_Rect win={WINDOW_W/2-170,WINDOW_H/2-120,340,240};
    SDL_Rect f_ip   ={win.x+120,win.y+50, 200,22};
    SDL_Rect f_port ={win.x+120,win.y+82, 200,22};
    SDL_Rect f_nb   ={win.x+120,win.y+114,200,22};
    SDL_Rect btn_ok ={win.x+win.w-180,win.y+win.h-38,80,26};
    SDL_Rect btn_bk ={win.x+win.w-90, win.y+win.h-38,80,26};

    while (1) {
        while (SDL_PollEvent(&e)) {
            if (e.type==SDL_QUIT){SDL_StopTextInput();return MENU_RESULT_QUIT;}
            if (e.type==SDL_MOUSEMOTION){mx=e.motion.x;my=e.motion.y;}
            if (e.type==SDL_MOUSEBUTTONDOWN&&e.button.button==SDL_BUTTON_LEFT){
                mx=e.button.x;my=e.button.y;
                if(!is_server&&mx>=f_ip.x&&mx<f_ip.x+f_ip.w&&my>=f_ip.y&&my<f_ip.y+f_ip.h) active=0;
                if(mx>=f_port.x&&mx<f_port.x+f_port.w&&my>=f_port.y&&my<f_port.y+f_port.h) active=1;
                if(is_server&&mx>=f_nb.x&&mx<f_nb.x+f_nb.w&&my>=f_nb.y&&my<f_nb.y+f_nb.h) active=2;
                if(mx>=btn_ok.x&&mx<btn_ok.x+btn_ok.w&&my>=btn_ok.y&&my<btn_ok.y+btn_ok.h) goto net_ok;
                if(mx>=btn_bk.x&&mx<btn_bk.x+btn_bk.w&&my>=btn_bk.y&&my<btn_bk.y+btn_bk.h){SDL_StopTextInput();return MENU_RESULT_BACK;}
            }
            if (e.type==SDL_TEXTINPUT){
                char *t=NULL; int ml=63;
                int digits_only=0;
                if(active==0&&!is_server){t=buf_ip;ml=63;}
                else if(active==1){t=buf_port;ml=5;digits_only=1;}
                else if(active==2&&is_server){t=buf_nb;ml=1;digits_only=1;}
                if(t&&(int)strlen(t)<ml){
                    char c=e.text.text[0];
                    if(!digits_only||(c>='0'&&c<='9'))
                        strncat(t,e.text.text,ml-strlen(t));
                }
            }
            if (e.type==SDL_KEYDOWN){
                if(e.key.keysym.sym==SDLK_BACKSPACE){
                    char *t=NULL;
                    if(active==0&&!is_server) t=buf_ip;
                    else if(active==1) t=buf_port;
                    else if(active==2&&is_server) t=buf_nb;
                    if(t&&strlen(t)>0) t[strlen(t)-1]=0;
                }
                if(e.key.keysym.sym==SDLK_TAB) active=(active+1)%3;
                if(e.key.keysym.sym==SDLK_RETURN) goto net_ok;
                if(e.key.keysym.sym==SDLK_ESCAPE){SDL_StopTextInput();return MENU_RESULT_BACK;}
            }
        }

        draw_desktop(ren);
        const char *title = is_server ? "HEBERGER - ZONIX.EXE" : "REJOINDRE - ZONIX.EXE";
        ros_window(ren,win,title,1);

        int lx=win.x+12, cy=win.y+28;
        if(!is_server){
            ros_label_sep(ren,lx,cy,"CONFIGURATION RESEAU");
            cy+=16;
            render_string(ren,lx,cy+7,"IP SERVEUR :",ROS_BLACK,1);
            ros_input_field(ren,f_ip,buf_ip,active==0);
            cy+=32;
        }
        render_string(ren,lx,f_port.y+7,"PORT :",ROS_BLACK,1);
        ros_input_field(ren,f_port,buf_port,active==1);
        if(is_server){
            render_string(ren,lx,f_nb.y+7,"NB JOUEURS :",ROS_BLACK,1);
            ros_input_field(ren,f_nb,buf_nb,active==2);
        }

        ros_button_ex(ren,btn_ok,"LANCER",mx,my,0);
        ros_button_ex(ren,btn_bk,"ANNULER",mx,my,0);

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }

net_ok:
    SDL_StopTextInput();
    strncpy(cfg->ip,buf_ip,63);
    cfg->port=atoi(buf_port); if(!cfg->port) cfg->port=SERVER_PORT;
    cfg->player_count=atoi(buf_nb); if(cfg->player_count<2)cfg->player_count=2;
    if(cfg->player_count>4)cfg->player_count=4;
    return is_server?MENU_RESULT_SERVER:MENU_RESULT_CLIENT;
}

void menu_tutorial(SDL_Renderer *ren) {
    SDL_Event e;
    SDL_Rect win={WINDOW_W/2-200,30,400,WINDOW_H-60};

    typedef struct{const char*txt;SDL_Color col;}Line;
    Line lines[]={
        {"OBJECTIF",               ROS_NAVY},
        {"Conquiere le plus grand",ROS_BLACK},
        {"territoire possible.",   ROS_BLACK},
        {"",                       ROS_BLACK},
        {"CONTROLES",              ROS_NAVY},
        {"Z/HAUT    : Monter",     ROS_BLACK},
        {"S/BAS     : Descendre",  ROS_BLACK},
        {"Q/GAUCHE  : Gauche",     ROS_BLACK},
        {"D/DROITE  : Droite",     ROS_BLACK},
        {"ECHAP     : Quitter",    ROS_BLACK},
        {"",                       ROS_BLACK},
        {"REGLES",                 ROS_NAVY},
        {"Sors de ton territoire", ROS_BLACK},
        {"pour tracer un trail.",  ROS_BLACK},
        {"Reviens pour capturer",  ROS_BLACK},
        {"la zone fermee.",        ROS_BLACK},
        {"",                       ROS_BLACK},
        {"TU MEURS SI...",         ROS_ALERT},
        {"- Tu touches ton trail", ROS_BLACK},
        {"- Tu sors de la carte",  ROS_BLACK},
        {"- Un ennemi te coupe",   ROS_BLACK},
        {"",                       ROS_BLACK},
        {"POWER-UPS",              ROS_NAVY},
        {"[V] Vitesse x2 (5s)",    ROS_BLACK},
        {"[S] Bouclier (3s)",      ROS_BLACK},
        {"[B] Bombe (rayon 4)",    ROS_BLACK},
        {"[T] Territoire 5x5",     ROS_BLACK},
        {"",                       ROS_BLACK},
        {"Appuie sur une touche",  ROS_DKGRAY},
    };
    int n=(int)(sizeof(lines)/sizeof(lines[0]));

    int running=1;
    while(running){
        while(running && SDL_PollEvent(&e)){
            if(e.type==SDL_QUIT||e.type==SDL_KEYDOWN||e.type==SDL_MOUSEBUTTONDOWN)
                running=0;
        }
        if(!running) break;
        draw_desktop(ren);
        ros_window(ren,win,"AIDE - ZONIX.EXE",1);
        SDL_Rect content={win.x+4,win.y+25,win.w-8,win.h-30};
        ros_fill(ren,content,ROS_PAPER);
        int ty=win.y+30;
        for(int i=0;i<n;i++){
            if(!lines[i].txt[0]){ty+=6;continue;}
            if(lines[i].col.b==128&&lines[i].col.r==0){
                SDL_Rect hl={win.x+4,ty-1,win.w-8,11};
                SDL_SetRenderDrawColor(ren,0,0,128,255);
                SDL_RenderFillRect(ren,&hl);
                render_string(ren,win.x+8,ty,lines[i].txt,ROS_WHITE,1);
            } else {
                render_string(ren,win.x+10,ty,lines[i].txt,lines[i].col,1);
            }
            ty+=12;
            if(ty>win.y+win.h-16) break;
        }
        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }
}

void menu_waiting(SDL_Renderer *ren, int connected, int expected) {
    draw_desktop(ren);
    SDL_Rect win={WINDOW_W/2-140,WINDOW_H/2-60,280,120};
    ros_window(ren,win,"ZONIX.EXE - En attente",1);
    render_string(ren,win.x+12,win.y+28,"EN ATTENTE DE JOUEURS...",(SDL_Color){0,0,128,255},1);
    char buf[32]; snprintf(buf,32,"%d / %d CONNECTES",connected,expected);
    render_string(ren,win.x+12,win.y+42,buf,ROS_BLACK,1);
    int bw=win.w-24;
    SDL_Rect bar_bg={win.x+12,win.y+58,bw,14};
    ros_bevel(ren,bar_bg,0);
    int fill=expected>0?bw*connected/expected:0;
    SDL_SetRenderDrawColor(ren,0,119,255,255);
    SDL_Rect bar_fill={bar_bg.x+2,bar_bg.y+2,fill>0?fill-4:0,bar_bg.h-4};
    SDL_RenderFillRect(ren,&bar_fill);
    if(fill>0){
        for(int x=bar_bg.x+2;x<bar_bg.x+fill-2;x+=8){
            SDL_SetRenderDrawColor(ren,0,80,200,255);
            SDL_Rect seg={x,bar_bg.y+2,6,bar_bg.h-4};
            SDL_RenderFillRect(ren,&seg);
        }
    }
    SDL_RenderPresent(ren);
}

void menu_connecting(SDL_Renderer *ren, const char *ip, int port) {
    draw_desktop(ren);
    SDL_Rect win={WINDOW_W/2-150,WINDOW_H/2-50,300,100};
    ros_window(ren,win,"Connexion en cours...",1);
    render_string(ren,win.x+12,win.y+28,"Connexion au serveur...",(SDL_Color){0,0,128,255},1);
    char buf[80]; snprintf(buf,80,"IP: %s  PORT: %d",ip,port);
    for(int k=0;buf[k];k++) if(buf[k]>='a'&&buf[k]<='z') buf[k]-=32;
    render_string(ren,win.x+12,win.y+44,buf,ROS_BLACK,1);
    static int anim=0; anim=(anim+4)%200;
    SDL_Rect bar={win.x+12,win.y+62,win.w-24,12};
    ros_bevel(ren,bar,0);
    SDL_SetRenderDrawColor(ren,0,119,255,255);
    SDL_Rect seg={bar.x+2+anim%(bar.w-20),bar.y+2,20,bar.h-4};
    SDL_RenderFillRect(ren,&seg);
    SDL_RenderPresent(ren);
}