# Makefile - ZONIX Paper.io 2.5D
CC     = gcc
CFLAGS = -Wall -Wextra -g -I./include -O2 -D_WIN32_WINNT=0x0600

LIBS_SERVER = -lpthread -lws2_32
LIBS_CLIENT = -lmingw32 -lSDL2main -lSDL2 -lpthread -mwindows -lws2_32 -lm

# Commun : logique pure, PAS de SDL
SRC_COMMON = src/map.c src/player.c src/game.c src/flood_fill.c \
             src/powerup.c src/colors.c src/bot.c

# Serveur : console, pas de SDL
SRC_SERVER = src/main_server.c src/server.c src/threads.c

# Client : SDL2 + audio + FX + assets
SRC_CLIENT = src/main_client.c src/client.c \
             src/render.c src/menu.c src/lobby.c \
             src/audio.c src/fx.c src/assets.c

.PHONY: all server client clean

all: server client

server: $(SRC_SERVER) $(SRC_COMMON)
	$(CC) $(CFLAGS) -o server.exe $(SRC_SERVER) $(SRC_COMMON) $(LIBS_SERVER)
	@echo [OK] server.exe compile

client: $(SRC_CLIENT) $(SRC_COMMON)
	$(CC) $(CFLAGS) -o client.exe $(SRC_CLIENT) $(SRC_COMMON) $(LIBS_CLIENT)
	@echo [OK] client.exe compile

clean:
	-del /f server.exe client.exe 2>NUL
	@echo [OK] Nettoyage