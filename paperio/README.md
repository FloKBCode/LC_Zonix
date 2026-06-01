# ZONIX — Paper.io 2.5D Multijoueur

> Jeu arcade réseau local codé en C avec SDL2  
> TP n°3 — Jeu Multi Joueurs — Ynov Bachelor 1

---

## Présentation

ZONIX est un Paper.io jouable en réseau local (TCP/IP, switch ethernet) pour 2 à 4 joueurs.  
Le rendu est en **2.5D** : chaque case est dessinée comme un bloc avec une ombre portée,  
donnant une illusion de volume sans aucune bibliothèque 3D.

---

## Architecture

```
paperio/
├── include/          Headers (partagés client + serveur)
│   ├── constants.h   Taille grille, port, tick rate, shadow offset
│   ├── types.h       GameState, Player, Packet
│   ├── colors.h      Palette 2.5D des 4 joueurs
│   ├── map.h         Accès au tableau 2D
│   ├── player.h      Déplacement, trail, mort
│   ├── game.h        Logique du jeu (update par tick)
│   ├── flood_fill.h  Capture de territoire (BFS)
│   ├── render.h      Rendu SDL2 2.5D + police pixel art
│   ├── menu.h        Menus, config réseau, tutoriel
│   ├── server.h      Serveur TCP
│   ├── client.h      Client TCP
│   └── threads.h     4 threads du serveur
├── src/
│   ├── map.c         Implémentation carte
│   ├── player.c      Implémentation joueur
│   ├── game.c        Logique : init, update, game_over
│   ├── flood_fill.c  BFS pour capturer l'intérieur
│   ├── render.c      Rendu 2.5D + HUD + écran fin
│   ├── menu.c        Tous les écrans menu
│   ├── server.c      Serveur TCP (bind/listen/accept)
│   ├── client.c      Client TCP (connect/send/recv)
│   ├── threads.c     4 threads : accept/receive/game_loop/send
│   ├── main_server.c Point d'entrée serveur
│   └── main_client.c Point d'entrée client (boucle jeu)
├── Makefile
└── SDL2.dll          (à conserver dans ce dossier)
```

---

## Compilation

```bash
make all       # compile server.exe et client.exe
make server    # seulement le serveur
make client    # seulement le client
make clean     # supprime les executables
```

**Pré-requis :** MinGW (gcc), SDL2.dll dans le dossier (déjà présent)

---

## Lancement

### Sur la machine serveur
```
server.exe
```
→ Menu SDL2 → choisir nombre de joueurs (2-4) + port → LANCER  
→ Le serveur attend les connexions puis lance la partie automatiquement

### Sur chaque machine cliente
```
client.exe
```
→ Menu → REJOINDRE → saisir l'IP du serveur + port → LANCER  
→ Attente des autres joueurs → partie lancée dès que tout le monde est connecté

---

## Contrôles

| Touche              | Action       |
|---------------------|--------------|
| ↑ / Z / W           | Monter       |
| ↓ / S               | Descendre    |
| ← / Q / A           | Gauche       |
| → / D               | Droite       |
| Espace (game over)  | Retour menu  |
| Echap               | Quitter      |

---

## Règles

1. Tu pars d'un territoire initial (carré 9×9)
2. **Sors** de ton territoire pour tracer une trainée
3. **Reviens** sur ton territoire pour capturer tout l'intérieur de ta boucle
4. **Tu meurs si** : tu touches ton propre trail, un bord, ou un ennemi te coupe
5. **Coupe le trail d'un ennemi** pour le tuer directement
6. Le gagnant est le joueur avec le plus grand territoire

---

## Réseau

- Protocole : **TCP** (fiabilité garantie)
- Architecture : **client/serveur centralisé**
- Topologie : réseau local avec switch ethernet
- Port par défaut : **5555** (configurable dans le menu)
- Le serveur gère la logique du jeu (authoritative server)
- Les clients n'envoient que leur direction (anti-triche)

### Schéma réseau

```
[Client P1] ──┐
[Client P2] ──┤── [Switch] ── [Serveur]
[Client P3] ──┤
[Client P4] ──┘
```

---

## Technique : Rendu 2.5D

Le rendu 2.5D est réalisé en **SDL2 pur** (pas de SDL2_ttf, pas de SDL2_image) :

- Chaque case = un rectangle "top" + une ombre portée (bas + droite)
- L'offset de l'ombre (`SHADOW_X=3, SHADOW_Y=3`) donne l'effet de volume
- Le trail est plus haut que le territoire (`lift=3`)
- La tête du joueur est encore plus haute (`lift=5`) et légèrement agrandie
- Le joueur local a un point blanc au centre de sa tête
- La police de jeu est une **bitmap 5×7 intégrée** dans `render.c` (A-Z + chiffres)

---

## Barème (objectif 20/20)

| Critère                        | Points | Status |
|-------------------------------|--------|--------|
| 4 joueurs en réseau           | 8/8    | ✅     |
| Menu config réseau            | 2/2    | ✅     |
| Tutoriel de jeu               | 2/2    | ✅     |
| Jeu en 2.5D                   | 2/2*   | ✅     |
| Fonctionne pendant présentation| 3/3   | ✅     |
| Note des autres groupes       | 5/5    | 🎯     |

*2.5D comptabilisé comme 2D (2pts). La 3D aurait nécessité OpenGL.

---

## Équipe

- **Marly** : `render.c`, `menu.c`, `main_client.c`, `colors.h`
- **Florence** : `server.c`, `client.c`, `threads.c`, `main_server.c`
- **Sarah** : `game.c`, `player.c`, `map.c`, `flood_fill.c`
