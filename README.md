# ZONIX — Paper.io 2.5D Multijoueur

> Jeu arcade réseau local en C / SDL2  
> TP n°3 — Jeu Multi Joueurs — Ynov Bachelor 1 — 2025

---

## Présentation

ZONIX est un clone de Paper.io jouable en **réseau local** (TCP/IP, switch ethernet) pour **2 à 4 joueurs** humains ou bots.

Le rendu est en **2.5D** : chaque case est dessinée comme un bloc avec ombre portée, simulant le volume sans aucune bibliothèque 3D. L'interface est dans le style **Retro_OS** (Windows 98).

---

## Barème estimé

| Critère | Points | Status |
|---------|--------|--------|
| 4 joueurs réseau (humains + bots) | 8/8 | ✅ |
| Menu config réseau (IP, port) | 2/2 | ✅ |
| Tutoriel intégré | 2/2 | ✅ |
| Rendu 2.5D (shadow offset SDL2 pur) | 2/2 | ✅ |
| Fonctionne en présentation | 3/3 | ✅ |
| Note des autres groupes | ~/5 | 🎯 |

---

## Architecture

```
paperio/
├── include/        Headers partagés client + serveur
├── src/
│   ├── map.c       Grille 2D, calcul scores
│   ├── player.c    Déplacement, trail, mort
│   ├── game.c      Logique tick, countdown, powerups
│   ├── flood_fill.c BFS capture de territoire
│   ├── powerup.c   Spawn, effets, ramassage
│   ├── bot.c       IA FSM 5 états
│   ├── colors.c    Palette 12 couleurs + 5 thèmes
│   ├── render.c    Rendu 2.5D, police Soft, podium
│   ├── menu.c      Menus Retro_OS Win98
│   ├── lobby.c     Lobby pseudo/couleur/thème/taille
│   ├── audio.c     Sons synthétiques SDL2 natif
│   ├── server.c    Serveur TCP avec restart auto
│   ├── client.c    Client TCP non-bloquant
│   ├── threads.c   4 threads + mutex + anti-spoof
│   ├── main_server.c Point d'entrée serveur (console)
│   └── main_client.c Point d'entrée client (SDL2)
└── Makefile
```

---

## Compilation

```bash
make all       # server.exe + client.exe
make server    # serveur seulement
make client    # client seulement
make clean
```

**Prérequis :** MinGW (gcc), SDL2.dll dans le dossier

---

## Lancement

### Serveur (machine hôte)
```
.\server.exe
```
→ Nombre de joueurs humains → Total humains+bots → Port (5555)

### Client (chaque machine)
```
.\client.exe
```
→ REJOINDRE → IP du serveur → port 5555 → Lobby → JOUER

---

## Contrôles

| Touche | Action |
|--------|--------|
| Z/↑ | Monter |
| S/↓ | Descendre |
| Q/← | Gauche |
| D/→ | Droite |
| Echap | Quitter |
| Espace (fin) | Rejouer |

---

## Fonctionnalités

### Gameplay
- Trail + capture par BFS (flood fill depuis les bords)
- 4 power-ups : Vitesse (V), Bouclier (S), Bombe (B), Territoire (T)
- Bouclier de spawn 3s, countdown 3-2-1
- Anti-demi-tour côté serveur

### Réseau
- Architecture client/serveur TCP autoritatif
- 4 threads : accept, receive, game_loop, send
- Mutex sur game_state (thread-safe)
- Anti-spoof : identité = socket, pas le paquet client
- Restart automatique après chaque partie

### Bot (FSM 5 états)
- **EXPAND** : boucles rectangulaires pour capturer du territoire
- **ATTACK** : fonce sur le trail ennemi à portée
- **DEFEND** : rentre si trail > 20 cases
- **FLEE** : fuit si ennemi < 4 cases en étant exposé
- **RANDOM** : déblocage si progression nulle

### Interface
- Style **Retro_OS** (Windows 98) : fenêtres biseautées, bureau sarcelle
- 5 thèmes : Dark, Retro_OS, Synthwave, Minimal, Girly
- 12 couleurs joueur choisissables
- 3 tailles de terrain : Petit (60×45), Normal (80×60)
- Police bitmap **Soft** maison 7×9px (arrondie, sans SDL2_ttf)
- HUD avec barres de progression
- Podium 1er/2e/3e en fin de partie
- Sons synthétiques SDL2 natif (sans SDL2_mixer)

### Sons
| Événement | Son |
|-----------|-----|
| Début de partie | Fanfare 3 notes |
| Capture territoire | Bip montant |
| Mort | Descente dramatique |
| Kill ennemi | Bip grave |
| Power-up | Triple bip |
| Countdown | Tick |
| Game over | Mélodie descendante |

---

## Schéma réseau

```
[Client P1] ──┐
[Client P2] ──┤── [Switch ethernet] ── [Serveur]
[Client P3] ──┤
[Bot P4]    ──┘ (géré par le serveur)
```

---

## Équipe

- **Marly** — `render.c`, `menu.c`, `main_client.c`, `colors.c`, `audio.c`, `lobby.c`
- **Florence** — `server.c`, `client.c`, `threads.c`, `main_server.c`, `game.c`, `player.c`, `map.c`, `flood_fill.c`, `bot.c`