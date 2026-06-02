// audio.h - Architecture audio ZONIX
// Pour activer l'audio : installer SDL2_mixer et décommenter SDL_MIXER_ENABLED
// dans le Makefile puis placer les fichiers dans assets/sounds/
//
// Structure des assets audio :
//   assets/sounds/music_menu.wav       -- musique menu principal
//   assets/sounds/music_game.wav       -- musique en jeu
//   assets/sounds/fx_capture.wav       -- son capture territoire
//   assets/sounds/fx_kill.wav          -- son kill ennemi
//   assets/sounds/fx_death.wav         -- son mort du joueur
//   assets/sounds/fx_powerup.wav       -- son ramassage power-up
//   assets/sounds/fx_countdown.wav     -- son tick countdown
//   assets/sounds/fx_start.wav         -- son début de partie
//   assets/sounds/fx_gameover.wav      -- son fin de partie
//   assets/sounds/ui_click.wav         -- son clic bouton
//   assets/sounds/ui_hover.wav         -- son survol bouton

#ifndef AUDIO_H
#define AUDIO_H

// Décommente pour activer l'audio (nécessite SDL2_mixer)
// #define SDL_MIXER_ENABLED

typedef enum {
    SFX_CAPTURE = 0,
    SFX_KILL,
    SFX_DEATH,
    SFX_POWERUP,
    SFX_COUNTDOWN,
    SFX_START,
    SFX_GAMEOVER,
    SFX_UI_CLICK,
    SFX_UI_HOVER,
    SFX_COUNT
} SfxId;

typedef enum {
    MUSIC_MENU = 0,
    MUSIC_GAME,
    MUSIC_COUNT
} MusicId;

// Config audio (modifiable sans recompiler via assets/audio_config.txt)
typedef struct {
    float master_vol;   // 0.0 - 1.0
    float music_vol;
    float sfx_vol;
    int   music_enabled;
    int   sfx_enabled;
} AudioConfig;

// --- API ---
// Toutes les fonctions sont des no-ops si SDL_MIXER_ENABLED n'est pas défini

int  audio_init(void);          // initialise SDL2_mixer
void audio_shutdown(void);
void audio_play_sfx(SfxId id);  // joue un effet sonore
void audio_play_music(MusicId id, int loop); // 0=once, 1=loop
void audio_stop_music(void);
void audio_set_volume(float master, float music, float sfx);
void audio_load_config(const char *path);
int  audio_is_muted(void);  // retourne 1 si son coupe  // charge assets/audio_config.txt

#endif