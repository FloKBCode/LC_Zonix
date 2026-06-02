// bot.c - FSM contextuel agressif : capture rapide + attaque les trails
#include <stdlib.h>
#include <string.h>
#include "../include/bot.h"
#include "../include/map.h"
#include "../include/player.h"
#include "../include/constants.h"

typedef enum {
    STATE_EXPAND,   // fait des boucles pour capturer du territoire
    STATE_ATTACK,   // fonce sur le trail d'un ennemi pour le tuer
    STATE_DEFEND,   // rentre chez lui si trail trop long
    STATE_FLEE,     // fuit si ennemi très proche
    STATE_RANDOM,   // deblocage
} BotState;

typedef struct {
    BotState state;
    int      steps;
    int      turn_dir;      // 0=droite 1=gauche
    int      stuck;         // compteur blocage
    int      last_score;
    int      random_steps;
    Direction random_dir;
    int      loop_size;     // taille de la boucle courante
    int      phase;         // phase dans la boucle (0=sortir, 1=cote1, 2=cote2, 3=cote3)
    int      phase_steps;   // pas dans la phase actuelle
} BotCtx;

static BotCtx bots[MAX_PLAYERS];

// ---- Utils ----
static Direction turn_right(Direction d){
    switch(d){case DIR_UP:return DIR_RIGHT;case DIR_RIGHT:return DIR_DOWN;
              case DIR_DOWN:return DIR_LEFT;default:return DIR_UP;}
}
static Direction turn_left(Direction d){
    switch(d){case DIR_UP:return DIR_LEFT;case DIR_LEFT:return DIR_DOWN;
              case DIR_DOWN:return DIR_RIGHT;default:return DIR_UP;}
}
static Direction reverse_dir(Direction d){
    switch(d){case DIR_UP:return DIR_DOWN;case DIR_DOWN:return DIR_UP;
              case DIR_LEFT:return DIR_RIGHT;default:return DIR_LEFT;}
}

static int dist(int x1,int y1,int x2,int y2){
    int dx=x1-x2; if(dx<0)dx=-dx;
    int dy=y1-y2; if(dy<0)dy=-dy;
    return dx+dy;
}

// Vérifie si avancer dans la direction d depuis (x,y) est safe (lookahead cases)
static int is_safe(GameState *gs, int pid, int x, int y, Direction d, int lookahead){
    int nx=x, ny=y;
    for(int s=0;s<lookahead;s++){
        switch(d){case DIR_UP:ny--;break;case DIR_DOWN:ny++;break;
                  case DIR_LEFT:nx--;break;default:nx++;break;}
        if(!map_in_bounds_gs(gs,nx,ny)) return 0;
        if(player_trail_contains(&gs->players[pid-1],nx,ny)) return 0;
        for(int j=0;j<gs->player_count;j++){
            if(j==pid-1||!gs->players[j].alive) continue;
            if(player_trail_contains(&gs->players[j],nx,ny)) return 0;
            if(s==0&&gs->players[j].x==nx&&gs->players[j].y==ny) return 0;
        }
    }
    return 1;
}

// Meilleure direction safe parmi 4
static Direction safest(GameState *gs, int pid, int x, int y, Direction cur){
    Direction tries[4]={cur,turn_right(cur),turn_left(cur),reverse_dir(cur)};
    for(int t=0;t<4;t++)
        if(is_safe(gs,pid,x,y,tries[t],1)) return tries[t];
    return cur;
}

// Direction vers une cible
static Direction toward(int fx,int fy,int tx,int ty,Direction cur){
    int dx=tx-fx, dy=ty-fy;
    if(abs(dx)>abs(dy)) return dx>0?DIR_RIGHT:DIR_LEFT;
    if(dy!=0)            return dy>0?DIR_DOWN:DIR_UP;
    return cur;
}

// Trouve le trail ennemi le plus proche
static int find_enemy_trail(GameState *gs, int pid, int *ex, int *ey){
    Player *me=&gs->players[pid-1];
    int best=-1, bd=99999;
    for(int j=0;j<gs->player_count;j++){
        if(j==pid-1||!gs->players[j].alive) continue;
        Player *en=&gs->players[j];
        for(int t=0;t<en->trail_len;t++){
            int d2=dist(me->x,me->y,en->trail[t][0],en->trail[t][1]);
            if(d2<bd){bd=d2;best=j;*ex=en->trail[t][0];*ey=en->trail[t][1];}
        }
    }
    return best>=0?bd:-1;
}

// Trouve la case de territoire propre la plus proche
static void find_home(GameState *gs, int pid, int *hx, int *hy){
    Player *me=&gs->players[pid-1];
    int gw=gs->grid_w>0?gs->grid_w:GRID_W;
    int gh=gs->grid_h>0?gs->grid_h:GRID_H;
    int best=99999;
    *hx=me->x; *hy=me->y;
    for(int y=0;y<gh;y+=2)
        for(int x=0;x<gw;x+=2)
            if(gs->map[y][x]==pid){
                int d2=dist(me->x,me->y,x,y);
                if(d2<best){best=d2;*hx=x;*hy=y;}
            }
}

void bot_init(int pid){
    int i=pid-1;
    memset(&bots[i],0,sizeof(BotCtx));
    bots[i].state    =STATE_EXPAND;
    bots[i].turn_dir =pid%2;
    bots[i].loop_size=8+rand()%5;
}

Direction bot_think(GameState *gs, int pid){
    int i=pid-1;
    Player *me=&gs->players[i];
    BotCtx *ctx=&bots[i];
    Direction d=me->dir;

    ctx->steps++;

    // Détection blocage
    if(me->score<=ctx->last_score) ctx->stuck++;
    else ctx->stuck=0;
    ctx->last_score=me->score;

    // Décrement random
    if(ctx->state==STATE_RANDOM&&ctx->random_steps>0)
        ctx->random_steps--;

    // --- Analyse contextuelle ---
    int ex=0,ey=0;
    int trail_dist=find_enemy_trail(gs,pid,&ex,&ey);

    int on_own=(map_get(gs,me->x,me->y)==pid);

    // Transitions
    if(ctx->stuck>6){
        ctx->state=STATE_RANDOM;
        ctx->random_steps=6+rand()%8;
        ctx->random_dir=safest(gs,pid,me->x,me->y,d);
        ctx->stuck=0;
    } else if(me->trail_len>20){
        ctx->state=STATE_DEFEND;
    } else if(trail_dist>=0&&trail_dist<6&&me->trail_len==0){
        // Ennemi avec trail proche et on est safe sur notre territoire → ATTACK
        ctx->state=STATE_ATTACK;
    } else if(trail_dist>=0&&trail_dist<4&&me->trail_len>5){
        // Ennemi proche et on est exposé → FLEE
        ctx->state=STATE_FLEE;
    } else if(ctx->state==STATE_RANDOM&&ctx->random_steps<=0){
        ctx->state=STATE_EXPAND;
        ctx->phase=0; ctx->phase_steps=0;
    } else if(ctx->state==STATE_DEFEND&&on_own&&me->trail_len==0){
        ctx->state=STATE_EXPAND;
        ctx->phase=0; ctx->phase_steps=0;
        ctx->loop_size=8+rand()%6;
    } else if(ctx->state==STATE_ATTACK&&(trail_dist<0||trail_dist>10)){
        ctx->state=STATE_EXPAND;
    }

    // --- Comportement ---
    switch(ctx->state){

        case STATE_EXPAND:{
            // Boucle rectangulaire structurée en 4 phases
            // Phase 0 : sort du territoire (tout droit)
            // Phase 1-3 : 3 côtés du rectangle puis retour
            int side=ctx->loop_size;
            ctx->phase_steps++;

            if(ctx->phase==0){
                // Sort du territoire pendant 'side/2' pas
                if(!on_own && ctx->phase_steps>=2){
                    ctx->phase=1;
                    ctx->phase_steps=0;
                    d=ctx->turn_dir?turn_left(d):turn_right(d);
                }
                // Reste sur la direction courante
            } else {
                // Chaque côté = side pas, puis tourne
                if(ctx->phase_steps>=side){
                    ctx->phase_steps=0;
                    ctx->phase++;
                    if(ctx->phase>3){
                        ctx->phase=0;
                        ctx->turn_dir=!ctx->turn_dir;
                        ctx->loop_size=7+rand()%7;
                    } else {
                        d=ctx->turn_dir?turn_left(d):turn_right(d);
                    }
                }
            }
            break;
        }

        case STATE_ATTACK:{
            // Fonce sur le trail ennemi
            Direction t=toward(me->x,me->y,ex,ey,d);
            if(is_safe(gs,pid,me->x,me->y,t,1)) d=t;
            else d=safest(gs,pid,me->x,me->y,d);
            break;
        }

        case STATE_DEFEND:{
            // Rentre chez soi le plus vite possible
            int hx,hy;
            find_home(gs,pid,&hx,&hy);
            Direction t=toward(me->x,me->y,hx,hy,d);
            if(is_safe(gs,pid,me->x,me->y,t,1)) d=t;
            else d=safest(gs,pid,me->x,me->y,d);
            break;
        }

        case STATE_FLEE:{
            // Fuit l'ennemi le plus proche
            int ned=-1,ndist=99999;
            for(int j=0;j<gs->player_count;j++){
                if(j==i||!gs->players[j].alive) continue;
                int d2=dist(me->x,me->y,gs->players[j].x,gs->players[j].y);
                if(d2<ndist){ndist=d2;ned=j;}
            }
            if(ned>=0){
                Direction away=reverse_dir(
                    toward(gs->players[ned].x,gs->players[ned].y,
                           me->x,me->y,d));
                if(is_safe(gs,pid,me->x,me->y,away,1)) d=away;
                else d=safest(gs,pid,me->x,me->y,d);
            }
            break;
        }

        case STATE_RANDOM:{
            d=is_safe(gs,pid,me->x,me->y,ctx->random_dir,1)
              ?ctx->random_dir
              :safest(gs,pid,me->x,me->y,d);
            break;
        }
    }

    // Filet de securite
    if(!is_safe(gs,pid,me->x,me->y,d,1)){
        d=safest(gs,pid,me->x,me->y,d);
        ctx->state=STATE_EXPAND;
        ctx->phase=0; ctx->phase_steps=0;
    }

    return d;
}