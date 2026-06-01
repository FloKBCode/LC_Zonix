# Makefile - ZONIX Paper.io 2.5D
# Usage : mingw32-make all   (depuis le dossier paperio/)

CC     = gcc
CFLAGS = -Wall -Wextra -g -I./include -O2 -D_WIN32_WINNT=0x0600

# Serveur : PAS de SDL2 (console pure)
LIBS_SERVER = -lpthread -lws2_32

# Client : SDL2 + reseau
LIBS_CLIENT = -lmingw32 -lSDL2main -lSDL2 -lpthread -mwindows -lws2_32

SRC_COMMON = src/map.c src/player.c src/game.c src/flood_fill.c

SRC_SERVER = src/main_server.c src/server.c src/threads.c

SRC_CLIENT = src/main_client.c src/client.c \
             src/render.c src/menu.c

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
	@echo [OK] Nettoyage termine
