// render.c - Police Soft 7x9 + rendu 2.5D Retro_OS + podium CORRIGE
#include <SDL2/SDL.h>
#include <string.h>
#include <stdio.h>
#include "../include/render.h"
#include "../include/constants.h"
#include "../include/colors.h"
#include "../include/types.h"

// ============================================================
// POLICE SOFT 7x9
// ============================================================
#define FONT_SOFT_H    9
#define FONT_SOFT_W    7
#define FONT_SOFT_FIRST 32

static const unsigned short FONT_SOFT[59][7] = {
{0x000,0x000,0x000,0x000,0x000,0x000,0x000},
{0x000,0x000,0x1DF,0x1DF,0x1DF,0x000,0x000},
{0x000,0x000,0x000,0x000,0x000,0x000,0x000},
{0x000,0x000,0x000,0x000,0x000,0x000,0x000},
{0x000,0x000,0x000,0x000,0x000,0x000,0x000},
{0x063,0x073,0x018,0x00C,0x026,0x063,0x041},
{0x000,0x000,0x000,0x000,0x000,0x000,0x000},
{0x000,0x000,0x000,0x000,0x000,0x000,0x000},
{0x000,0x000,0x000,0x000,0x000,0x000,0x000},
{0x000,0x000,0x000,0x000,0x000,0x000,0x000},
{0x000,0x000,0x000,0x000,0x000,0x000,0x000},
{0x000,0x000,0x000,0x000,0x000,0x000,0x000},
{0x000,0x000,0x000,0x000,0x000,0x000,0x000},
{0x018,0x018,0x018,0x018,0x018,0x018,0x018},
{0x000,0x000,0x180,0x180,0x000,0x000,0x000},
{0x060,0x070,0x058,0x00C,0x007,0x003,0x001},
{0x0FE,0x1FF,0x1E3,0x1BB,0x18F,0x1FF,0x0FE},
{0x184,0x186,0x1FF,0x1FF,0x1FF,0x180,0x180},
{0x1E2,0x1F3,0x1B3,0x1B3,0x1B3,0x1BF,0x19E},
{0x183,0x1B3,0x1B3,0x1B3,0x1B3,0x1FF,0x0FE},
{0x01F,0x01F,0x018,0x018,0x018,0x1FF,0x1FF},
{0x1BF,0x1BF,0x1B3,0x1B3,0x1B3,0x1F3,0x0E3},
{0x0FE,0x1FF,0x1B3,0x1B3,0x1B3,0x1F3,0x0E2},
{0x003,0x003,0x1E3,0x1F3,0x01B,0x00F,0x007},
{0x0EE,0x1FF,0x1B3,0x1B3,0x1B3,0x1FF,0x0EE},
{0x09E,0x1BF,0x1B3,0x1B3,0x1B3,0x1FF,0x0FE},
{0x000,0x000,0x06C,0x06C,0x000,0x000,0x000},
{0x000,0x000,0x000,0x000,0x000,0x000,0x000},
{0x000,0x000,0x000,0x000,0x000,0x000,0x000},
{0x000,0x000,0x000,0x000,0x000,0x000,0x000},
{0x000,0x000,0x000,0x000,0x000,0x000,0x000},
{0x000,0x000,0x000,0x000,0x000,0x000,0x000},
{0x000,0x000,0x000,0x000,0x000,0x000,0x000},
{0x1FC,0x1FE,0x033,0x033,0x033,0x1FE,0x1FC},
{0x1FF,0x1FF,0x1B3,0x1B3,0x1B3,0x1FF,0x0EE},
{0x0FE,0x1FF,0x183,0x183,0x183,0x183,0x082},
{0x1FF,0x1FF,0x183,0x183,0x1C7,0x0FE,0x07C},
{0x1FF,0x1FF,0x1B3,0x1B3,0x1B3,0x183,0x183},
{0x1FF,0x1FF,0x033,0x033,0x033,0x003,0x003},
{0x0FE,0x1FF,0x183,0x183,0x1B3,0x1F3,0x0F2},
{0x1FF,0x1FF,0x030,0x030,0x030,0x1FF,0x1FF},
{0x183,0x183,0x1FF,0x1FF,0x1FF,0x183,0x183},
{0x0E0,0x1E0,0x180,0x183,0x183,0x1FF,0x0FF},
{0x1FF,0x1FF,0x038,0x06C,0x0C6,0x183,0x101},
{0x1FF,0x1FF,0x180,0x180,0x180,0x180,0x180},
{0x1FF,0x1FF,0x006,0x00C,0x006,0x1FF,0x1FF},
{0x1FF,0x1FF,0x006,0x00C,0x018,0x1FF,0x1FF},
{0x0FE,0x1FF,0x183,0x183,0x183,0x1FF,0x0FE},
{0x1FF,0x1FF,0x033,0x033,0x033,0x03F,0x01E},
{0x0FE,0x1FF,0x183,0x1A3,0x1C3,0x1FF,0x17E},
{0x1FF,0x1FF,0x033,0x073,0x0F3,0x1BF,0x11E},
{0x09E,0x1BF,0x1B3,0x1B3,0x1B3,0x1F3,0x0F2},
{0x003,0x003,0x1FF,0x1FF,0x1FF,0x003,0x003},
{0x0FF,0x1FF,0x180,0x180,0x180,0x1FF,0x0FF},
{0x01F,0x07F,0x1E0,0x180,0x1E0,0x07F,0x01F},
{0x1FF,0x1FF,0x060,0x030,0x060,0x1FF,0x1FF},
{0x183,0x1EF,0x07C,0x010,0x07C,0x1EF,0x183},
{0x003,0x00F,0x1FC,0x1F8,0x1FC,0x00F,0x003},
{0x1C3,0x1E3,0x1B3,0x19B,0x18F,0x187,0x183},
};


void render_char(SDL_Renderer *ren, int x, int y, char c, SDL_Color col, int scale){
    if(c>='a'&&c<='z') c=c-'a'+'A';
    if((unsigned char)c<FONT_SOFT_FIRST) c=' ';
    int idx=(unsigned char)c-FONT_SOFT_FIRST;
    if(idx<0||idx>=59) idx=0;
    SDL_SetRenderDrawColor(ren,col.r,col.g,col.b,col.a);
    for(int ci=0;ci<FONT_SOFT_W;ci++){
        unsigned short cd=FONT_SOFT[idx][ci];
        for(int row=0;row<FONT_SOFT_H;row++)
            if(cd&(1<<row)){
                SDL_Rect px={x+ci*scale,y+row*scale,scale,scale};
                SDL_RenderFillRect(ren,&px);
            }
    }
}

void render_string(SDL_Renderer *ren, int x, int y, const char *str, SDL_Color col, int scale){
    int cx=x;
    while(*str){ render_char(ren,cx,y,*str,col,scale); cx+=(FONT_SOFT_W+1)*scale; str++; }
}

static void render_str_center(SDL_Renderer *ren, int cx, int y, const char *str, SDL_Color col, int sc){
    render_string(ren,cx-text_w(str,sc)/2,y,str,col,sc);
}

static void render_str_shadow(SDL_Renderer *ren, int x, int y, const char *str,
                               SDL_Color col, SDL_Color shd, int sc){
    render_string(ren,x+sc,y+sc,str,shd,sc);
    render_string(ren,x,y,str,col,sc);
}

// ============================================================
// RETRO_OS helpers
// ============================================================
#define ROS_WHITE  ((SDL_Color){255,255,255,255})
#define ROS_LTGRAY ((SDL_Color){192,192,192,255})
#define ROS_DKGRAY ((SDL_Color){80,80,80,255})
#define ROS_BLACK  ((SDL_Color){0,0,0,255})
#define ROS_NAVY   ((SDL_Color){0,0,128,255})
#define ROS_ACCENT ((SDL_Color){0,119,255,255})
#define ROS_PAPER  ((SDL_Color){245,245,240,255})

static int is_retro(Theme th){ return th==THEME_RETRO; }

static void ros_bevel(SDL_Renderer *ren, SDL_Rect r, int raised){
    SDL_Color hi=raised?ROS_WHITE:ROS_DKGRAY, lo=raised?ROS_DKGRAY:ROS_WHITE;
    SDL_Color hi2=raised?ROS_LTGRAY:(SDL_Color){50,50,50,255};
    SDL_Color lo2=raised?(SDL_Color){50,50,50,255}:ROS_LTGRAY;
    SDL_SetRenderDrawColor(ren,ROS_LTGRAY.r,ROS_LTGRAY.g,ROS_LTGRAY.b,255);
    SDL_RenderFillRect(ren,&r);
    SDL_SetRenderDrawColor(ren,hi.r,hi.g,hi.b,255);
    SDL_RenderDrawLine(ren,r.x,r.y,r.x+r.w-1,r.y);
    SDL_RenderDrawLine(ren,r.x,r.y,r.x,r.y+r.h-1);
    SDL_SetRenderDrawColor(ren,lo2.r,lo2.g,lo2.b,255);
    SDL_RenderDrawLine(ren,r.x+r.w-1,r.y,r.x+r.w-1,r.y+r.h-1);
    SDL_RenderDrawLine(ren,r.x,r.y+r.h-1,r.x+r.w-1,r.y+r.h-1);
    SDL_SetRenderDrawColor(ren,hi2.r,hi2.g,hi2.b,255);
    SDL_RenderDrawLine(ren,r.x+1,r.y+1,r.x+r.w-2,r.y+1);
    SDL_RenderDrawLine(ren,r.x+1,r.y+1,r.x+1,r.y+r.h-2);
    SDL_SetRenderDrawColor(ren,lo.r,lo.g,lo.b,255);
    SDL_RenderDrawLine(ren,r.x+r.w-2,r.y+1,r.x+r.w-2,r.y+r.h-2);
    SDL_RenderDrawLine(ren,r.x+1,r.y+r.h-2,r.x+r.w-2,r.y+r.h-2);
}

static void ros_window(SDL_Renderer *ren, SDL_Rect r, const char *title){
    ros_bevel(ren,r,1);
    SDL_Rect tb={r.x+3,r.y+3,r.w-6,18};
    SDL_SetRenderDrawColor(ren,0,0,128,255); SDL_RenderFillRect(ren,&tb);
    SDL_SetRenderDrawColor(ren,30,30,160,255);
    SDL_RenderDrawLine(ren,tb.x+1,tb.y+1,tb.x+tb.w-2,tb.y+1);
    render_string(ren,r.x+7,r.y+6,title,ROS_WHITE,1);
    SDL_Rect xb={r.x+r.w-20,r.y+4,16,14}; ros_bevel(ren,xb,1);
    render_string(ren,xb.x+4,xb.y+3,"X",ROS_BLACK,1);
}

// ============================================================
// BLOC 2.5D
// ============================================================
void render_block_25d(SDL_Renderer *ren, int xp, int yp, int sz, int lift,
                      SDL_Color tc, SDL_Color sc2){
    SDL_SetRenderDrawColor(ren,sc2.r,sc2.g,sc2.b,sc2.a);
    SDL_Rect f={xp+SHADOW_X,yp+sz-lift,sz,SHADOW_Y+lift}; SDL_RenderFillRect(ren,&f);
    SDL_Rect rf={xp+sz,yp+SHADOW_Y-lift,SHADOW_X,sz}; SDL_RenderFillRect(ren,&rf);
    SDL_SetRenderDrawColor(ren,tc.r,tc.g,tc.b,tc.a);
    SDL_Rect top={xp,yp-lift,sz,sz}; SDL_RenderFillRect(ren,&top);
}

static void render_arrow(SDL_Renderer *ren, int cx, int cy, Direction dir, SDL_Color col){
    SDL_SetRenderDrawColor(ren,col.r,col.g,col.b,col.a);
    switch(dir){
        case DIR_UP:   SDL_RenderDrawLine(ren,cx,cy-3,cx-2,cy); SDL_RenderDrawLine(ren,cx,cy-3,cx+2,cy); break;
        case DIR_DOWN: SDL_RenderDrawLine(ren,cx,cy+3,cx-2,cy); SDL_RenderDrawLine(ren,cx,cy+3,cx+2,cy); break;
        case DIR_LEFT: SDL_RenderDrawLine(ren,cx-3,cy,cx,cy-2); SDL_RenderDrawLine(ren,cx-3,cy,cx,cy+2); break;
        default:       SDL_RenderDrawLine(ren,cx+3,cy,cx,cy-2); SDL_RenderDrawLine(ren,cx+3,cy,cx,cy+2); break;
    }
}

// ============================================================
// INIT
// ============================================================
int render_init(SDL_Window **win, SDL_Renderer **ren){
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)!=0) return -1;
    *win=SDL_CreateWindow("ZONIX - Paper.io 2.5D",
        SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
        WINDOW_W+SHADOW_X+4,WINDOW_H+SHADOW_Y+44,SDL_WINDOW_SHOWN);
    if(!*win) return -1;
    *ren=SDL_CreateRenderer(*win,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if(!*ren) return -1;
    SDL_SetRenderDrawBlendMode(*ren,SDL_BLENDMODE_NONE);
    return 0;
}

// ============================================================
// RENDU JEU
// ============================================================
void render_draw(SDL_Renderer *ren, GameState *gs, int my_id){
    Theme th=gs->theme;
    ThemeColors tc=THEMES[th];
    int retro=is_retro(th);
    SDL_SetRenderDrawColor(ren,tc.bg_top.r,tc.bg_top.g,tc.bg_top.b,255);
    SDL_RenderClear(ren);
    if(retro){
        SDL_SetRenderDrawColor(ren,156,163,176,255);
        for(int y=0;y<WINDOW_H;y+=4) for(int x=0;x<WINDOW_W;x+=4) SDL_RenderDrawPoint(ren,x,y);
    }
    int gw=gs->grid_w>0?gs->grid_w:GRID_W, gh=gs->grid_h>0?gs->grid_h:GRID_H;
    for(int y=0;y<gh;y++) for(int x=0;x<gw;x++){
        int owner=gs->map[y][x];
        if(owner==0){
            if(retro){ SDL_SetRenderDrawColor(ren,200,205,210,255); SDL_Rect r={x*CELL_SIZE,y*CELL_SIZE,CELL_SIZE-1,CELL_SIZE-1}; SDL_RenderFillRect(ren,&r); }
            else render_block_25d(ren,x*CELL_SIZE,y*CELL_SIZE,CELL_SIZE-1,1,tc.bg_top,tc.bg_shadow);
        } else {
            int ci=gs->players[owner-1].color_idx;
            SDL_Color top2=COLOR_PALETTE[ci].territory, shd=COLOR_PALETTE[ci].shadow;
            if(retro){
                SDL_SetRenderDrawColor(ren,top2.r,top2.g,top2.b,255);
                SDL_Rect r={x*CELL_SIZE,y*CELL_SIZE,CELL_SIZE-1,CELL_SIZE-1}; SDL_RenderFillRect(ren,&r);
                SDL_Color hi={(Uint8)SDL_min(top2.r+60,255),(Uint8)SDL_min(top2.g+60,255),(Uint8)SDL_min(top2.b+60,255),255};
                SDL_SetRenderDrawColor(ren,hi.r,hi.g,hi.b,255);
                SDL_RenderDrawLine(ren,x*CELL_SIZE,y*CELL_SIZE,x*CELL_SIZE+CELL_SIZE-2,y*CELL_SIZE);
                SDL_RenderDrawLine(ren,x*CELL_SIZE,y*CELL_SIZE,x*CELL_SIZE,y*CELL_SIZE+CELL_SIZE-2);
                SDL_SetRenderDrawColor(ren,shd.r,shd.g,shd.b,255);
                SDL_RenderDrawLine(ren,x*CELL_SIZE+CELL_SIZE-2,y*CELL_SIZE,x*CELL_SIZE+CELL_SIZE-2,y*CELL_SIZE+CELL_SIZE-2);
                SDL_RenderDrawLine(ren,x*CELL_SIZE,y*CELL_SIZE+CELL_SIZE-2,x*CELL_SIZE+CELL_SIZE-2,y*CELL_SIZE+CELL_SIZE-2);
            } else render_block_25d(ren,x*CELL_SIZE,y*CELL_SIZE,CELL_SIZE-1,2,top2,shd);
        }
    }
    for(int i=0;i<gs->player_count;i++){
        Player *p=&gs->players[i]; if(!p->alive) continue;
        int ci=p->color_idx;
        for(int t=0;t<p->trail_len;t++){
            int tx=p->trail[t][0],ty=p->trail[t][1];
            if(retro){ SDL_Color tc2=COLOR_PALETTE[ci].trail; SDL_SetRenderDrawColor(ren,tc2.r,tc2.g,tc2.b,255); SDL_Rect r={tx*CELL_SIZE+1,ty*CELL_SIZE+1,CELL_SIZE-3,CELL_SIZE-3}; SDL_RenderFillRect(ren,&r); }
            else render_block_25d(ren,tx*CELL_SIZE,ty*CELL_SIZE,CELL_SIZE-1,3,COLOR_PALETTE[ci].trail,COLOR_PALETTE[ci].trail_shadow);
        }
    }
    for(int i=0;i<MAX_POWERUPS;i++){
        PowerUp *pu=&gs->powerups[i]; if(!pu->active) continue;
        if(pu->lifetime<20&&(pu->lifetime%4)<2) continue;
        PupColor pc=POWERUP_COLORS[pu->type];
        if(retro){ SDL_Rect pr={pu->x*CELL_SIZE,pu->y*CELL_SIZE,CELL_SIZE,CELL_SIZE}; ros_bevel(ren,pr,1); render_char(ren,pu->x*CELL_SIZE+1,pu->y*CELL_SIZE+1,pc.icon[0],(SDL_Color){0,0,128,255},1); }
        else { render_block_25d(ren,pu->x*CELL_SIZE,pu->y*CELL_SIZE,CELL_SIZE-1,4,pc.top,pc.shadow); render_char(ren,pu->x*CELL_SIZE+1,pu->y*CELL_SIZE-3,pc.icon[0],(SDL_Color){255,255,255,255},1); }
    }
    for(int i=0;i<gs->player_count;i++){
        Player *p=&gs->players[i]; if(!p->alive) continue;
        int ci=p->color_idx, extra=(my_id>0&&i+1==my_id)?1:0;
        int lift=HEAD_LIFT+3+extra, sz=CELL_SIZE-1+extra*2;
        int ox=p->x*CELL_SIZE-extra, oy=p->y*CELL_SIZE-extra;
        if(p->shield_ticks>0){ SDL_Color sc={80,220,255,255}; if(retro) sc=ROS_ACCENT; SDL_SetRenderDrawColor(ren,sc.r,sc.g,sc.b,255); SDL_Rect a={ox-2,oy-(retro?0:lift)-2,sz+4,sz+4}; SDL_RenderDrawRect(ren,&a); }
        if(p->speed_ticks>0){ SDL_SetRenderDrawColor(ren,255,220,0,255); SDL_Rect a={ox-1,oy-(retro?0:lift)-1,sz+2,sz+2}; SDL_RenderDrawRect(ren,&a); }
        if(retro){ SDL_Rect hr={ox,oy,sz+1,sz+1}; SDL_Color hc=COLOR_PALETTE[ci].head; SDL_SetRenderDrawColor(ren,hc.r,hc.g,hc.b,255); SDL_RenderFillRect(ren,&hr); ros_bevel(ren,hr,1); }
        else render_block_25d(ren,ox,oy,sz,lift,COLOR_PALETTE[ci].head,COLOR_PALETTE[ci].head_shadow);
        render_arrow(ren,ox+sz/2,oy-(retro?0:lift)+sz/2,p->dir,retro?ROS_BLACK:(SDL_Color){30,30,30,255});
        if(my_id>0&&i+1==my_id){ SDL_SetRenderDrawColor(ren,retro?0:255,retro?0:255,retro?128:255,255); SDL_Rect dot={ox+sz/2-1,oy-(retro?0:lift)+sz/2-1,2,2}; SDL_RenderFillRect(ren,&dot); }
        if(p->pseudo[0]!='\0'){
            SDL_Color pcol=retro?ROS_BLACK:COLOR_PALETTE[ci].territory;
            if(retro){ int pw=text_w(p->pseudo,1)+2; SDL_SetRenderDrawColor(ren,255,255,255,200); SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_BLEND); SDL_Rect pb={ox,oy-11,pw,10}; SDL_RenderFillRect(ren,&pb); SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_NONE); }
            render_string(ren,ox+1,oy-(retro?11:lift+11),p->pseudo,pcol,1);
        }
    }
    if(gs->countdown>0){
        SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(ren,retro?192:0,retro?192:0,retro?192:0,160);
        SDL_Rect ov={0,0,WINDOW_W+SHADOW_X+4,WINDOW_H+SHADOW_Y+44}; SDL_RenderFillRect(ren,&ov);
        SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_NONE);
        int sec=(gs->countdown/(1000/TICK_RATE))+1; if(sec<1)sec=1;
        char buf[4]; snprintf(buf,4,"%d",sec);
        if(retro){ SDL_Rect win={WINDOW_W/2-70,WINDOW_H/2-40,140,80}; ros_window(ren,win,"ZONIX.EXE"); render_str_center(ren,win.x+70,win.y+28,buf,(SDL_Color){0,0,128,255},4); render_str_center(ren,win.x+70,win.y+62,"GET READY",ROS_DKGRAY,1); }
        else { render_str_center(ren,WINDOW_W/2,WINDOW_H/2-16,buf,tc.countdown,4); render_str_center(ren,WINDOW_W/2,WINDOW_H/2+20,"GET READY",(SDL_Color){220,220,220,255},1); }
    }
    render_hud(ren,gs,my_id);
    SDL_RenderPresent(ren);
}

// ============================================================
// HUD
// ============================================================
void render_hud(SDL_Renderer *ren, GameState *gs, int my_id){
    int hud_y=WINDOW_H+SHADOW_Y;
    int retro=is_retro(gs->theme);
    int total=(gs->grid_w>0?gs->grid_w:GRID_W)*(gs->grid_h>0?gs->grid_h:GRID_H);
    if(retro){ SDL_Rect hb={0,hud_y,WINDOW_W+SHADOW_X+4,44}; SDL_SetRenderDrawColor(ren,ROS_LTGRAY.r,ROS_LTGRAY.g,ROS_LTGRAY.b,255); SDL_RenderFillRect(ren,&hb); SDL_SetRenderDrawColor(ren,ROS_DKGRAY.r,ROS_DKGRAY.g,ROS_DKGRAY.b,255); SDL_RenderDrawLine(ren,0,hud_y,WINDOW_W+SHADOW_X+4,hud_y); SDL_SetRenderDrawColor(ren,ROS_WHITE.r,ROS_WHITE.g,ROS_WHITE.b,255); SDL_RenderDrawLine(ren,0,hud_y+1,WINDOW_W+SHADOW_X+4,hud_y+1); }
    else { ThemeColors tc=THEMES[gs->theme]; SDL_SetRenderDrawColor(ren,tc.hud_bg.r,tc.hud_bg.g,tc.hud_bg.b,255); SDL_Rect hb={0,hud_y,WINDOW_W+SHADOW_X+4,44}; SDL_RenderFillRect(ren,&hb); }
    for(int i=0;i<gs->player_count;i++){
        Player *p=&gs->players[i];
        int ci=p->color_idx, xoff=i*((WINDOW_W)/gs->player_count)+4;
        SDL_Color col=COLOR_PALETTE[ci].territory;
        int pct=total>0?p->score*100/total:0;
        if(retro){
            SDL_Rect panel={xoff,hud_y+4,WINDOW_W/gs->player_count-6,34}; ros_bevel(ren,panel,0);
            SDL_SetRenderDrawColor(ren,col.r,col.g,col.b,255); SDL_Rect sq={xoff+3,hud_y+8,8,8}; SDL_RenderFillRect(ren,&sq);
            SDL_SetRenderDrawColor(ren,0,0,0,255); SDL_RenderDrawRect(ren,&sq);
            if(my_id>0&&i+1==my_id){ SDL_SetRenderDrawColor(ren,0,0,128,255); SDL_Rect sel={xoff+2,hud_y+7,10,10}; SDL_RenderDrawRect(ren,&sel); }
            char name[16]; if(p->pseudo[0]!='\0') snprintf(name,16,"%s",p->pseudo); else snprintf(name,16,"P%d",i+1);
            SDL_Color tc2=p->alive?ROS_BLACK:ROS_DKGRAY;
            render_string(ren,xoff+14,hud_y+7,name,tc2,1);
            char num[8]; if(p->alive) snprintf(num,8,"%d%%",pct); else snprintf(num,8,"MORT");
            render_string(ren,xoff+14,hud_y+19,num,p->alive?(SDL_Color){0,0,128,255}:ROS_DKGRAY,1);
            if(p->alive&&pct>0){ SDL_Rect bg={xoff+3,hud_y+30,panel.w-6,5}; SDL_SetRenderDrawColor(ren,ROS_DKGRAY.r,ROS_DKGRAY.g,ROS_DKGRAY.b,255); SDL_RenderFillRect(ren,&bg); int bw=bg.w*pct/100; SDL_SetRenderDrawColor(ren,0,119,255,255); SDL_Rect bf={bg.x,bg.y,bw,bg.h}; SDL_RenderFillRect(ren,&bf); }
        } else {
            ThemeColors tc2=THEMES[gs->theme];
            if(my_id>0&&i+1==my_id){ SDL_SetRenderDrawColor(ren,255,255,255,255); SDL_Rect out={xoff-1,hud_y+5,10,10}; SDL_RenderFillRect(ren,&out); }
            SDL_SetRenderDrawColor(ren,col.r,col.g,col.b,255); SDL_Rect sq={xoff,hud_y+6,8,8}; SDL_RenderFillRect(ren,&sq);
            char label[16]; if(p->pseudo[0]!='\0') snprintf(label,16,"%s",p->pseudo); else snprintf(label,16,"P%d",i+1);
            SDL_Color tcol=p->alive?col:(SDL_Color){80,80,80,255};
            render_string(ren,xoff+12,hud_y+6,label,tcol,1);
            char num[8]; if(p->alive) snprintf(num,8,"%d%%",pct); else snprintf(num,8,"MORT");
            render_string(ren,xoff+12,hud_y+18,num,tcol,1);
            (void)tc2;
        }
    }
}

// ============================================================
// GAME OVER — podium CORRIGE, layout fixe indépendant du nb joueurs
// ============================================================
void render_game_over(SDL_Renderer *ren, int winner_id, GameState *gs){
    SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ren,is_retro(gs->theme)?192:0,is_retro(gs->theme)?192:0,is_retro(gs->theme)?192:0,180);
    SDL_Rect ov={0,0,WINDOW_W+SHADOW_X+4,WINDOW_H+SHADOW_Y+44}; SDL_RenderFillRect(ren,&ov);
    SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_NONE);

    // Fenêtre centrée fixe 480x340
    int ww=480, wh=340;
    SDL_Rect win={WINDOW_W/2-ww/2, WINDOW_H/2-wh/2, ww, wh};
    ros_window(ren,win,"RESULTATS FINAUX - ZONIX.EXE");

    // GAME OVER
    SDL_Color go_col=(SDL_Color){0,0,128,255};
    SDL_Color go_shd=(SDL_Color){100,100,180,255};
    render_str_shadow(ren,win.x+(ww-text_w("GAME OVER",2))/2,win.y+26,"GAME OVER",go_col,go_shd,2);

    // Gagnant
    if(winner_id>=1&&winner_id<=MAX_PLAYERS){
        int ci=gs->players[winner_id-1].color_idx;
        char name[16];
        if(gs->players[winner_id-1].pseudo[0]!='\0') snprintf(name,16,"%s",gs->players[winner_id-1].pseudo);
        else snprintf(name,16,"P%d",winner_id);
        for(int k=0;name[k];k++) if(name[k]>='a'&&name[k]<='z') name[k]-=32;
        char msg[32]; snprintf(msg,32,"%s GAGNE!",name);
        SDL_Color wc=COLOR_PALETTE[ci].territory, ws=COLOR_PALETTE[ci].shadow;
        render_str_shadow(ren,win.x+(ww-text_w(msg,2))/2,win.y+46,msg,wc,ws,2);
    }

    // Séparateur
    SDL_SetRenderDrawColor(ren,ROS_DKGRAY.r,ROS_DKGRAY.g,ROS_DKGRAY.b,255);
    SDL_RenderDrawLine(ren,win.x+8,win.y+70,win.x+ww-8,win.y+70);
    SDL_SetRenderDrawColor(ren,ROS_WHITE.r,ROS_WHITE.g,ROS_WHITE.b,255);
    SDL_RenderDrawLine(ren,win.x+8,win.y+71,win.x+ww-8,win.y+71);

    // ---- PODIUM : colonnes de largeur fixe 100px, max 4 joueurs ----
    // Tri par score
    int order[MAX_PLAYERS]={0,1,2,3};
    for(int a=0;a<gs->player_count-1;a++)
        for(int b=a+1;b<gs->player_count;b++)
            if(gs->players[order[b]].score>gs->players[order[a]].score){
                int tmp=order[a];order[a]=order[b];order[b]=tmp;
            }

    int total=gs->grid_w*gs->grid_h;
    int pod_col_w=100;  // largeur fixe par colonne
    int pod_x_start=win.x+(ww-pod_col_w*gs->player_count)/2;
    int pod_base_y=win.y+165; // bas du podium

    int heights[4]={70,52,38,26};
    SDL_Color medals[4]={{220,180,0,255},{180,180,180,255},{180,100,40,255},{100,100,100,255}};

    for(int r=0;r<gs->player_count;r++){
        int idx=order[r];
        if(idx>=gs->player_count) continue;
        Player *p=&gs->players[idx];
        int ci=p->color_idx;
        SDL_Color pc=COLOR_PALETTE[ci].territory;

        int px=pod_x_start+r*pod_col_w;
        int ph=heights[r];

        // Socle
        SDL_Rect pod={px+6,pod_base_y-ph,pod_col_w-12,ph};
        SDL_SetRenderDrawColor(ren,medals[r].r,medals[r].g,medals[r].b,255);
        SDL_RenderFillRect(ren,&pod);
        ros_bevel(ren,pod,1);
        SDL_SetRenderDrawColor(ren,medals[r].r,medals[r].g,medals[r].b,255);
        SDL_Rect pm={pod.x+2,pod.y+2,pod.w-4,pod.h-4}; SDL_RenderFillRect(ren,&pm);

        // Numéro centré
        char rank[3]; snprintf(rank,3,"%d",r+1);
        render_str_center(ren,px+pod_col_w/2,pod.y+(ph-18)/2,rank,ROS_WHITE,2);

        // Bloc couleur joueur au-dessus
        SDL_Rect pb={px+pod_col_w/2-14,pod.y-24,28,20};
        SDL_SetRenderDrawColor(ren,pc.r,pc.g,pc.b,255); SDL_RenderFillRect(ren,&pb);
        ros_bevel(ren,pb,1);

        // Pseudo
        char name[16];
        if(p->pseudo[0]!='\0') snprintf(name,16,"%s",p->pseudo);
        else snprintf(name,16,"P%d",idx+1);
        for(int k=0;name[k];k++) if(name[k]>='a'&&name[k]<='z') name[k]-=32;
        render_str_center(ren,px+pod_col_w/2,pod.y-38,name,pc,1);

        // % sous le socle
        int pct=total>0?p->score*100/total:0;
        char sc[8]; snprintf(sc,8,"%d%%",pct);
        render_str_center(ren,px+pod_col_w/2,pod_base_y+3,sc,ROS_BLACK,1);
    }

    // Séparateur
    int sy=win.y+200;
    SDL_SetRenderDrawColor(ren,ROS_DKGRAY.r,ROS_DKGRAY.g,ROS_DKGRAY.b,255);
    SDL_RenderDrawLine(ren,win.x+8,sy,win.x+ww-8,sy);
    SDL_SetRenderDrawColor(ren,ROS_WHITE.r,ROS_WHITE.g,ROS_WHITE.b,255);
    SDL_RenderDrawLine(ren,win.x+8,sy+1,win.x+ww-8,sy+1);
    sy+=8;

    // Stats — colonnes alignées
    int col1=win.x+12, col2=win.x+130, col3=win.x+210, col4=win.x+280;
    render_string(ren,col1,sy,"NOM",ROS_DKGRAY,1);
    render_string(ren,col2,sy,"TERRIT",ROS_DKGRAY,1);
    render_string(ren,col3,sy,"KILLS",ROS_DKGRAY,1);
    render_string(ren,col4,sy,"CAPTS",ROS_DKGRAY,1);
    sy+=12;

    for(int r=0;r<gs->player_count;r++){
        int idx=order[r];
        if(idx>=gs->player_count) continue;
        Player *p=&gs->players[idx];
        int ci=p->color_idx;
        int pct=total>0?p->score*100/total:0;
        char name[12];
        if(p->pseudo[0]!='\0') snprintf(name,12,"%s",p->pseudo);
        else snprintf(name,12,"P%d",idx+1);
        for(int k=0;name[k];k++) if(name[k]>='a'&&name[k]<='z') name[k]-=32;
        char pcts[8],ks[8],cs[8];
        snprintf(pcts,8,"%d%%",pct);
        snprintf(ks,8,"%d",p->kills);
        snprintf(cs,8,"%d",p->captures);

        SDL_Color rc=idx+1==winner_id?(SDL_Color){0,0,128,255}:ROS_BLACK;
        SDL_SetRenderDrawColor(ren,COLOR_PALETTE[ci].territory.r,COLOR_PALETTE[ci].territory.g,
                               COLOR_PALETTE[ci].territory.b,255);
        SDL_Rect dot={col1,sy+1,6,6}; SDL_RenderFillRect(ren,&dot);
        render_string(ren,col1+9,sy,name,rc,1);
        render_string(ren,col2,sy,pcts,rc,1);
        render_string(ren,col3,sy,ks,rc,1);
        render_string(ren,col4,sy,cs,rc,1);
        sy+=12;
    }

    // Boutons
    SDL_Rect btn_r={win.x+ww/2-105,win.y+wh-36,100,26};
    SDL_Rect btn_q={win.x+ww/2+5,  win.y+wh-36,100,26};
    ros_bevel(ren,btn_r,1);
    SDL_SetRenderDrawColor(ren,0,119,255,255);
    SDL_Rect brf={btn_r.x+2,btn_r.y+2,btn_r.w-4,btn_r.h-4}; SDL_RenderFillRect(ren,&brf);
    render_str_center(ren,btn_r.x+50,btn_r.y+9,"REJOUER",ROS_WHITE,1);
    ros_bevel(ren,btn_q,1);
    render_str_center(ren,btn_q.x+50,btn_q.y+9,"QUITTER",ROS_BLACK,1);

    SDL_RenderPresent(ren);
}

void render_cleanup(SDL_Window *win, SDL_Renderer *ren){
    SDL_DestroyRenderer(ren); SDL_DestroyWindow(win); SDL_Quit();
}