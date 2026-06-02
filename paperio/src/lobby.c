// lobby.c - Retro_OS UI Kit : lobby et stats fin de partie
#include <SDL2/SDL.h>
#include <string.h>
#include <stdio.h>
#include "../include/lobby.h"
#include "../include/render.h"
#include "../include/colors.h"
#include "../include/constants.h"

#define ROS_WHITE  ((SDL_Color){255,255,255,255})
#define ROS_LTGRAY ((SDL_Color){192,192,192,255})
#define ROS_DKGRAY ((SDL_Color){128,128,128,255})
#define ROS_BLACK  ((SDL_Color){0,0,0,255})
#define ROS_NAVY   ((SDL_Color){0,0,128,255})
#define ROS_ACCENT ((SDL_Color){0,119,255,255})
#define ROS_PAPER  ((SDL_Color){245,245,240,255})
#define ROS_ALERT  ((SDL_Color){220,53,69,255})

static void ros_fill(SDL_Renderer *ren, SDL_Rect r, SDL_Color c) {
    SDL_SetRenderDrawColor(ren,c.r,c.g,c.b,255); SDL_RenderFillRect(ren,&r);
}

static void ros_bevel(SDL_Renderer *ren, SDL_Rect r, int raised) {
    SDL_Color hi=raised?ROS_WHITE:ROS_DKGRAY,lo=raised?ROS_DKGRAY:ROS_WHITE;
    SDL_Color hi2=raised?ROS_LTGRAY:(SDL_Color){80,80,80,255};
    SDL_Color lo2=raised?(SDL_Color){80,80,80,255}:ROS_LTGRAY;
    ros_fill(ren,r,ROS_LTGRAY);
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

static void ros_window(SDL_Renderer *ren, SDL_Rect r, const char *title) {
    ros_bevel(ren,r,1);
    SDL_Rect tb={r.x+3,r.y+3,r.w-6,18};
    SDL_SetRenderDrawColor(ren,0,0,128,255); SDL_RenderFillRect(ren,&tb);
    SDL_SetRenderDrawColor(ren,30,30,160,255);
    SDL_RenderDrawLine(ren,tb.x+1,tb.y+1,tb.x+tb.w-2,tb.y+1);
    render_string(ren,r.x+7,r.y+6,title,ROS_WHITE,1);
    SDL_Rect xb={r.x+r.w-20,r.y+4,16,14}; ros_bevel(ren,xb,1);
    render_string(ren,xb.x+4,xb.y+3,"X",ROS_BLACK,1);
}

static void ros_input_field(SDL_Renderer *ren, SDL_Rect r, const char *val, int active) {
    ros_fill(ren,r,ROS_PAPER); ros_bevel(ren,r,0);
    if(active){
        SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(ren,0,119,255,40);
        SDL_Rect in={r.x+2,r.y+2,r.w-4,r.h-4}; SDL_RenderFillRect(ren,&in);
        SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_NONE);
    }
    render_string(ren,r.x+4,r.y+(r.h-7)/2,val,ROS_BLACK,1);
    if(active&&(SDL_GetTicks()/400)%2==0){
        // CORRECTION UI-1 : Remplacement de strlen()*6 par text_w() pour recaler le curseur clignotant
        int cx=r.x+4+text_w(val,1); 
        SDL_SetRenderDrawColor(ren,0,0,0,255);
        SDL_RenderDrawLine(ren,cx,r.y+3,cx,r.y+r.h-4);
    }
}

static void draw_desktop(SDL_Renderer *ren) {
    SDL_SetRenderDrawColor(ren,0,128,128,255); SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren,0,112,112,255);
    for(int y=0;y<WINDOW_H+44;y+=4)
        for(int x=0;x<WINDOW_W+8;x+=4)
            SDL_RenderDrawPoint(ren,x,y);
    SDL_Rect tb={0,WINDOW_H+SHADOW_Y+SHADOW_X,WINDOW_W+8,28};
    ros_bevel(ren,tb,1);
    SDL_Rect st={2,tb.y+2,70,22}; ros_bevel(ren,st,1);
    render_string(ren,st.x+4,st.y+7,"DEMARRER",ROS_BLACK,1);
    Uint32 t=SDL_GetTicks()/1000;
    char clk[8]; snprintf(clk,8,"%02d:%02d",(int)(t/60)%60,(int)(t%60));
    render_string(ren,WINDOW_W-40,tb.y+8,clk,ROS_BLACK,1);
}

static int btn_ex(SDL_Renderer *ren, SDL_Rect r, const char *lbl, int mx, int my) {
    int hov=(mx>=r.x&&mx<r.x+r.w&&my>=r.y&&my<r.y+r.h);
    if(hov){SDL_SetRenderDrawColor(ren,210,215,225,255);SDL_RenderFillRect(ren,&r);}
    ros_bevel(ren,r,1);
    // CORRECTION UI-1 : Centrage parfait des libellés de boutons
    int tw=text_w(lbl,1); 
    render_string(ren,r.x+(r.w-tw)/2,r.y+(r.h-7)/2,lbl,ROS_BLACK,1);
    return hov;
}

// ============================================================
// TABS (onglets Win98)
// ============================================================
static const char *TAB_NAMES[4]={"IDENTITE","COULEUR","TERRAIN","THEME"};

int lobby_screen(SDL_Renderer *ren, LobbyInfo *info, int player_id) {
    SDL_Event e;
    int mx=0,my=0,tab=0;

    if(!info->pseudo[0]) snprintf(info->pseudo,16,"P%d",player_id);
    if(info->color_idx<0||info->color_idx>=COLOR_PALETTE_SIZE)
        info->color_idx=(player_id-1)%COLOR_PALETTE_SIZE;

    char buf_pseudo[16]; strncpy(buf_pseudo,info->pseudo,15); buf_pseudo[15]=0;
    SDL_StartTextInput();

    SDL_Rect win={WINDOW_W/2-210,40,420,WINDOW_H-80};
    SDL_Rect tab_area={win.x+4,win.y+25,win.w-8,18};
    SDL_Rect content={win.x+4,win.y+43,win.w-8,win.h-80};
    SDL_Rect btn_ok={win.x+win.w-105,win.y+win.h-36,100,26};

    while(1){
        while(SDL_PollEvent(&e)){
            if(e.type==SDL_QUIT){SDL_StopTextInput();return -1;}
            if(e.type==SDL_MOUSEMOTION){mx=e.motion.x;my=e.motion.y;}
            if(e.type==SDL_TEXTINPUT&&tab==0){
                int l=(int)strlen(buf_pseudo);
                if(l<10&&e.text.text[0]>=32&&e.text.text[0]<127)
                    strncat(buf_pseudo,e.text.text,15-strlen(buf_pseudo));
            }
            if(e.type==SDL_KEYDOWN){
                if(e.key.keysym.sym==SDLK_BACKSPACE&&tab==0&&strlen(buf_pseudo)>0)
                    buf_pseudo[strlen(buf_pseudo)-1]=0;
                if(e.key.keysym.sym==SDLK_RETURN) goto lobby_done;
                if(e.key.keysym.sym==SDLK_ESCAPE){SDL_StopTextInput();return -1;}
                if(e.key.keysym.sym==SDLK_TAB) tab=(tab+1)%4;
            }
            if(e.type==SDL_MOUSEBUTTONDOWN&&e.button.button==SDL_BUTTON_LEFT){
                mx=e.button.x;my=e.button.y;
                
                // Clic onglet
                for(int t=0;t<4;t++){
                    // CORRECTION UI-1 : Alignement des hitboxes de clic sur la géométrie réelle 8 px
                    int tw2=text_w(TAB_NAMES[t],1)+12; 
                    int tx=tab_area.x;
                    for(int tt=0;tt<t;tt++) tx+=text_w(TAB_NAMES[tt],1)+14; 
                    if(mx>=tx&&mx<tx+tw2&&my>=tab_area.y&&my<tab_area.y+18) tab=t;
                }
                
                // Clic couleur
                if(tab==1){
                    for(int c=0;c<COLOR_PALETTE_SIZE;c++){
                        int cx2=content.x+8+(c%6)*60;
                        int cy2=content.y+8+(c/6)*50;
                        if(mx>=cx2&&mx<cx2+50&&my>=cy2&&my<cy2+40)
                            info->color_idx=c;
                    }
                }
                
                // Clic taille
                if(tab==2){
                    // CORRECTION UI-2 : s < 2 empêche de sélectionner le choix "GRAND" cassé
                    for(int s=0;s<2;s++){ 
                        SDL_Rect br={content.x+8+s*130,content.y+20,120,32};
                        if(mx>=br.x&&mx<br.x+br.w&&my>=br.y&&my<br.y+br.h)
                            info->grid_size=s;
                    }
                }
                
                // Clic thème
                if(tab==3){
                    for(int t2=0;t2<THEME_COUNT;t2++){
                        SDL_Rect br={content.x+8+(t2%3)*130,content.y+8+(t2/3)*52,120,44};
                        if(mx>=br.x&&mx<br.x+br.w&&my>=br.y&&my<br.y+br.h)
                            info->theme=(Theme)t2;
                    }
                }
                
                if(mx>=btn_ok.x&&mx<btn_ok.x+btn_ok.w&&my>=btn_ok.y&&my<btn_ok.y+btn_ok.h)
                    goto lobby_done;
            }
        }

        draw_desktop(ren);
        char wtitle[32]; snprintf(wtitle,32,"JOUEUR %d - OPTIONS",player_id);
        ros_window(ren,win,wtitle);

        // --- Onglets ---
        int tx2=tab_area.x;
        for(int t=0;t<4;t++){
            // CORRECTION UI-1 : Calcul dynamique propre de l'espacement des onglets
            int tw2=text_w(TAB_NAMES[t],1)+12; 
            SDL_Rect tr={tx2,tab_area.y,tw2,18};
            if(t==tab){
                SDL_Rect ta={tr.x,tr.y-2,tr.w,tr.h+2};
                ros_fill(ren,ta,ROS_LTGRAY);
                SDL_SetRenderDrawColor(ren,ROS_WHITE.r,ROS_WHITE.g,ROS_WHITE.b,255);
                SDL_RenderDrawLine(ren,ta.x,ta.y,ta.x+ta.w-1,ta.y);
                SDL_RenderDrawLine(ren,ta.x,ta.y,ta.x,ta.y+ta.h);
                SDL_SetRenderDrawColor(ren,(SDL_Color){80,80,80,255}.r,(SDL_Color){80,80,80,255}.g,(SDL_Color){80,80,80,255}.b,255);
                SDL_RenderDrawLine(ren,ta.x+ta.w-1,ta.y,ta.x+ta.w-1,ta.y+ta.h);
                render_string(ren,ta.x+6,ta.y+5,TAB_NAMES[t],ROS_BLACK,1);
            } else {
                ros_bevel(ren,tr,1);
                render_string(ren,tr.x+6,tr.y+5,TAB_NAMES[t],ROS_DKGRAY,1);
            }
            tx2+=tw2+2;
        }

        ros_fill(ren,content,ROS_PAPER);
        ros_bevel(ren,content,0);

        int cx3=content.x+8, cy3=content.y+8;

        if(tab==0){
            render_string(ren,cx3,cy3,"Pseudo :",ROS_BLACK,1); cy3+=14;
            SDL_Rect pf={cx3,cy3,200,22};
            ros_input_field(ren,pf,buf_pseudo,1); cy3+=32;
            render_string(ren,cx3,cy3,"Apercu :",ROS_DKGRAY,1); cy3+=14;
            
            SDL_Color pc=COLOR_PALETTE[info->color_idx].territory;
            SDL_Rect prev={cx3,cy3,60,40};
            ros_bevel(ren,prev,1);
            SDL_SetRenderDrawColor(ren,pc.r,pc.g,pc.b,255);
            SDL_Rect pf2={prev.x+3,prev.y+3,prev.w-6,prev.h-6};
            SDL_RenderFillRect(ren,&pf2);
            render_string(ren,cx3+68,cy3+15,buf_pseudo,pc,1);
        }

        if(tab==1){
            for(int c=0;c<COLOR_PALETTE_SIZE;c++){
                int colx=content.x+8+(c%6)*60;
                int coly=content.y+8+(c/6)*50;
                SDL_Rect r_col={colx,coly,50,40};
                if(c==info->color_idx){
                    SDL_SetRenderDrawColor(ren,0,0,128,255);
                    SDL_Rect sel={colx-2,coly-2,54,44};
                    SDL_RenderDrawRect(ren,&sel);
                    SDL_RenderDrawRect(ren,&(SDL_Rect){colx-1,coly-1,52,42});
                }
                ros_bevel(ren,r_col,1);
                SDL_Color cc=COLOR_PALETTE[c].territory;
                SDL_SetRenderDrawColor(ren,cc.r,cc.g,cc.b,255);
                SDL_Rect cf={r_col.x+3,r_col.y+3,r_col.w-6,r_col.h-6};
                SDL_RenderFillRect(ren,&cf);
                render_string(ren,colx,coly+42,COLOR_PALETTE[c].name,
                              c==info->color_idx?ROS_NAVY:ROS_DKGRAY,1);
            }
        }

        if(tab==2){
            render_string(ren,cx3,cy3,"Taille du terrain :",ROS_BLACK,1);
            cy3+=20;
            const char *sizes2[]={"PETIT","NORMAL","GRAND"};
            const char *sizes3[]={"60x45","80x60","100x75"};
            // CORRECTION UI-2 : Limitation de l'affichage aux options PETIT et NORMAL (<= 2) pour sécuriser la démo
            for(int s=0;s<2;s++){ 
                SDL_Rect br={content.x+8+s*130,content.y+28,120,50};
                if(s==info->grid_size){
                    SDL_SetRenderDrawColor(ren,0,0,128,255);
                    SDL_Rect sel={br.x-2,br.y-2,br.w+4,br.h+4};
                    SDL_RenderDrawRect(ren,&sel);
                }
                ros_bevel(ren,br,s!=info->grid_size);
                // CORRECTION UI-1 : Centrage dynamique des textes du sélecteur de taille
                render_string(ren,br.x+(120-text_w(sizes2[s],1))/2, br.y+8,sizes2[s],ROS_BLACK,1);
                render_string(ren,br.x+(120-text_w(sizes3[s],1))/2, br.y+22,sizes3[s],ROS_DKGRAY,1);
            }
        }

        if(tab==3){
            const char *tnames[]={"DARK","RETRO_OS","SYNTHWAVE","MINIMAL","GIRLY"};
            for(int t2=0;t2<THEME_COUNT;t2++){
                SDL_Rect br={content.x+8+(t2%3)*130,content.y+8+(t2/3)*52,120,44};
                ThemeColors btc=THEMES[t2];
                ros_fill(ren,br,btc.bg_top);
                if((int)info->theme==t2){
                    SDL_SetRenderDrawColor(ren,0,0,128,255);
                    SDL_Rect sel={br.x-2,br.y-2,br.w+4,br.h+4};
                    SDL_RenderDrawRect(ren,&sel);
                    SDL_RenderDrawRect(ren,&(SDL_Rect){br.x-1,br.y-1,br.w+2,br.h+2});
                } else {
                    SDL_SetRenderDrawColor(ren,ROS_DKGRAY.r,ROS_DKGRAY.g,ROS_DKGRAY.b,255);
                    SDL_RenderDrawRect(ren,&br);
                }
                // CORRECTION UI-1 : Centrage du nom des thèmes graphiques
                render_string(ren,br.x+(120-text_w(tnames[t2],1))/2, br.y+18,tnames[t2],btc.hud_text,1);
            }
        }

        btn_ex(ren,btn_ok,"JOUER",mx,my);
        SDL_SetRenderDrawColor(ren,0,119,255,255);
        SDL_Rect okfill={btn_ok.x+2,btn_ok.y+2,btn_ok.w-4,btn_ok.h-4};
        SDL_RenderFillRect(ren,&okfill);
        // CORRECTION UI-1 : Centrage du bouton principal de validation
        int tw2=text_w("JOUER",1); 
        render_string(ren,btn_ok.x+(btn_ok.w-tw2)/2,btn_ok.y+9,"JOUER",ROS_WHITE,1);



        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }

lobby_done:
    SDL_StopTextInput();
    strncpy(info->pseudo,buf_pseudo,15); info->pseudo[15]=0;
    for(int k=0;info->pseudo[k];k++)
        if(info->pseudo[k]>='a'&&info->pseudo[k]<='z') info->pseudo[k]-=32;
    return 0;
}

// ============================================================
// STATS FIN DE PARTIE — tableau de scores Win98
// ============================================================
void lobby_show_stats(SDL_Renderer *ren, EndStats *stats, int my_id, Theme theme) {
    SDL_Event e;
    int running=1;
    (void)theme;

    SDL_Rect win={WINDOW_W/2-200,60,400,WINDOW_H-120};

    while(running){
        while(SDL_PollEvent(&e)){
            if(e.type==SDL_QUIT||e.type==SDL_KEYDOWN||e.type==SDL_MOUSEBUTTONDOWN)
                running=0;
        }

        draw_desktop(ren);
        ros_window(ren,win,"RESULTATS - ZONIX.EXE");

        int cx4=win.x+12, cy4=win.y+28;

        // Gagnant en vedette
        int w=stats->winner_id;
        if(w>=1&&w<=MAX_PLAYERS){
            char wname[32]; snprintf(wname,32,"%s GAGNE !",stats->pseudos[w-1]);
            for(int k=0;wname[k];k++) if(wname[k]>='a'&&wname[k]<='z') wname[k]-=32;
            // CORRECTION UI-1 : Calcul parfait de la largeur du rectangle de surlignage bleu
            SDL_Rect wb={cx4-2,cy4-2,text_w(wname,1)+8,14}; 
            ros_fill(ren,wb,ROS_NAVY);
            render_string(ren,cx4,cy4,wname,ROS_WHITE,1);
            cy4+=20;
        }

        SDL_SetRenderDrawColor(ren,ROS_DKGRAY.r,ROS_DKGRAY.g,ROS_DKGRAY.b,255);
        SDL_RenderDrawLine(ren,cx4,cy4,win.x+win.w-12,cy4);
        SDL_SetRenderDrawColor(ren,ROS_WHITE.r,ROS_WHITE.g,ROS_WHITE.b,255);
        SDL_RenderDrawLine(ren,cx4,cy4+1,win.x+win.w-12,cy4+1);
        cy4+=10;

        render_string(ren,cx4,cy4,"RG  NOM          TERR  KILLS  CAPTS  MAX",ROS_DKGRAY,1);
        cy4+=14;
        SDL_SetRenderDrawColor(ren,ROS_DKGRAY.r,ROS_DKGRAY.g,ROS_DKGRAY.b,255);
        SDL_RenderDrawLine(ren,cx4,cy4,win.x+win.w-12,cy4); cy4+=4;

        int order[MAX_PLAYERS]={0,1,2,3};
        for(int a=0;a<MAX_PLAYERS-1;a++)
            for(int b=a+1;b<MAX_PLAYERS;b++)
                if(stats->scores[order[b]]>stats->scores[order[a]]){
                    int tmp=order[a];order[a]=order[b];order[b]=tmp;
                }

        // CORRECTION UI-4 : Utilisation de la taille dynamique transmise dans la structure EndStats
        int total = stats->grid_w * stats->grid_h; 
        for(int r=0;r<MAX_PLAYERS;r++){
            int i=order[r];
            if(i>=MAX_PLAYERS) continue;

            if(i+1==my_id||i+1==stats->winner_id){
                SDL_Color hc=i+1==stats->winner_id?ROS_NAVY:(SDL_Color){0,100,180,255};
                SDL_SetRenderDrawColor(ren,hc.r,hc.g,hc.b,255);
                SDL_Rect hl={cx4-2,cy4-1,win.w-20,13};
                SDL_RenderFillRect(ren,&hl);
            }

            const char *medals[]={"1.","2.","3.","4."};
            int pct=total>0?stats->scores[i]*100/total:0;
            char name[16]; snprintf(name,16,"%-10s",stats->pseudos[i]);
            for(int k=0;name[k];k++) if(name[k]>='a'&&name[k]<='z') name[k]-=32;
            char line[64];
            snprintf(line,64,"%s  %s %3d%%   %2d     %2d     %3d",
                     medals[r],name,pct,stats->kills[i],
                     stats->captures[i],stats->max_territory[i]);

            SDL_Color rc=(i+1==stats->winner_id||i+1==my_id)?ROS_WHITE:ROS_BLACK;
            render_string(ren,cx4,cy4,line,rc,1);
            cy4+=16;
        }

        cy4+=8;
        SDL_SetRenderDrawColor(ren,ROS_DKGRAY.r,ROS_DKGRAY.g,ROS_DKGRAY.b,255);
        SDL_RenderDrawLine(ren,cx4,cy4,win.x+win.w-12,cy4); cy4+=8;



        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }
}