// audio.c - Sons synthétiques via SDL2 AudioDevice (sans SDL2_mixer)
// Génère des beeps/sons directement en PCM
#include <SDL2/SDL.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "../include/audio.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static SDL_AudioDeviceID audio_dev = 0;
static int audio_ok = 0;

// Buffer PCM 16-bit mono 44100Hz
#define SAMPLE_RATE 44100
#define MAX_SAMPLES (SAMPLE_RATE / 2)  // 0.5s max par son

static Sint16 audio_buf[MAX_SAMPLES];
static int    audio_buf_len = 0;

// Génère un son : fréquence freq Hz, durée ms, forme (0=carré 1=sinus 2=descente)
static void gen_sound(float freq, int ms, int waveform, float vol) {
    int samples = (SAMPLE_RATE * ms) / 1000;
    if(samples > MAX_SAMPLES) samples = MAX_SAMPLES;
    audio_buf_len = samples * sizeof(Sint16);

    for(int i = 0; i < samples; i++) {
        float t = (float)i / SAMPLE_RATE;
        float f = freq;
        // Descente de fréquence pour mort/bombe
        if(waveform == 2) f = freq * (1.0f - (float)i/samples * 0.7f);

        float sample = 0;
        if(waveform == 0) {
            // Carré
            sample = sinf(2*M_PI*f*t) > 0 ? 1.0f : -1.0f;
        } else {
            // Sinus
            sample = sinf(2*M_PI*f*t);
        }

        // Envelope : fade out sur les 20% finaux
        float env = 1.0f;
        if(i > samples*0.8f)
            env = 1.0f - (float)(i - (int)(samples*0.8f)) / (samples*0.2f);

        audio_buf[i] = (Sint16)(sample * env * vol * 28000);
    }
}

int audio_init(void) {
    if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
        printf("[AUDIO] SDL_INIT_AUDIO failed: %s\n", SDL_GetError());
        return -1;
    }
    SDL_AudioSpec want, have;
    SDL_memset(&want, 0, sizeof(want));
    want.freq     = SAMPLE_RATE;
    want.format   = AUDIO_S16SYS;
    want.channels = 1;
    want.samples  = 512;
    want.callback = NULL;

    audio_dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
    if(audio_dev == 0) {
        printf("[AUDIO] OpenAudioDevice failed: %s\n", SDL_GetError());
        return -1;
    }
    SDL_PauseAudioDevice(audio_dev, 0);
    audio_ok = 1;
    printf("[AUDIO] Initialise (freq=%d)\n", have.freq);
    return 0;
}

// Déclaration externe de l'état muet (défini dans menu.c)
extern int audio_is_muted(void);

void audio_play_sfx(SfxId id) {
    if(!audio_ok) return;
    if(audio_is_muted()) return;

    switch(id) {
        case SFX_CAPTURE:
            // Bip montant court : territoire capturé
            gen_sound(440, 80, 1, 0.6f);
            SDL_QueueAudio(audio_dev, audio_buf, audio_buf_len);
            gen_sound(660, 80, 1, 0.6f);
            SDL_QueueAudio(audio_dev, audio_buf, audio_buf_len);
            break;
        case SFX_KILL:
            // Bip agressif : ennemi tué
            gen_sound(200, 60, 0, 0.7f);
            SDL_QueueAudio(audio_dev, audio_buf, audio_buf_len);
            gen_sound(150, 80, 0, 0.7f);
            SDL_QueueAudio(audio_dev, audio_buf, audio_buf_len);
            break;
        case SFX_DEATH:
            // Descente dramatique : mort du joueur
            gen_sound(400, 300, 2, 0.8f);
            SDL_QueueAudio(audio_dev, audio_buf, audio_buf_len);
            break;
        case SFX_POWERUP:
            // Bip joyeux triple : power-up ramassé
            gen_sound(523, 60, 1, 0.5f);
            SDL_QueueAudio(audio_dev, audio_buf, audio_buf_len);
            gen_sound(659, 60, 1, 0.5f);
            SDL_QueueAudio(audio_dev, audio_buf, audio_buf_len);
            gen_sound(784, 100, 1, 0.5f);
            SDL_QueueAudio(audio_dev, audio_buf, audio_buf_len);
            break;
        case SFX_COUNTDOWN:
            // Tick court
            gen_sound(880, 50, 0, 0.4f);
            SDL_QueueAudio(audio_dev, audio_buf, audio_buf_len);
            break;
        case SFX_START:
            // Fanfare de début
            gen_sound(523, 100, 1, 0.6f); SDL_QueueAudio(audio_dev, audio_buf, audio_buf_len);
            gen_sound(659, 100, 1, 0.6f); SDL_QueueAudio(audio_dev, audio_buf, audio_buf_len);
            gen_sound(784, 200, 1, 0.7f); SDL_QueueAudio(audio_dev, audio_buf, audio_buf_len);
            break;
        case SFX_GAMEOVER:
            // Mélodie descendante
            gen_sound(523, 150, 1, 0.6f); SDL_QueueAudio(audio_dev, audio_buf, audio_buf_len);
            gen_sound(440, 150, 1, 0.6f); SDL_QueueAudio(audio_dev, audio_buf, audio_buf_len);
            gen_sound(349, 300, 1, 0.7f); SDL_QueueAudio(audio_dev, audio_buf, audio_buf_len);
            break;
        case SFX_UI_CLICK:
            gen_sound(300, 40, 0, 0.3f);
            SDL_QueueAudio(audio_dev, audio_buf, audio_buf_len);
            break;
        default: break;
    }
}

void audio_play_music(MusicId id, int loop) { (void)id; (void)loop; }
void audio_stop_music(void) {}
void audio_set_volume(float a, float b, float c) { (void)a;(void)b;(void)c; }
void audio_load_config(const char *p) { (void)p; }

void audio_shutdown(void) {
    if(audio_dev) {
        SDL_CloseAudioDevice(audio_dev);
        audio_dev = 0;
    }
    audio_ok = 0;
}