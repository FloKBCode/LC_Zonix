# Makefile - Paper.io
CC     = gcc
CFLAGS = -Wall -Wextra -g -I./include
LIBS_SERVER = -lpthread -lws2_32
LIBS_CLIENT = -lmingw32 -lSDL2main -lSDL2 -lpthread -mwindows -lws2_32

SRC_COMMON = src/map.c src/player.c src/game.c src/flood_fill.c
SRC_SERVER = src/main_server.c src/server.c src/threads.c
SRC_CLIENT = src/main_client.c src/client.c src/render.c src/menu.c

all: server client

server: $(SRC_SERVER) $(SRC_COMMON)
	$(CC) $(CFLAGS) -o server $(SRC_SERVER) $(SRC_COMMON) $(LIBS_SERVER)

client: $(SRC_CLIENT) $(SRC_COMMON)
	$(CC) $(CFLAGS) -o client $(SRC_CLIENT) $(SRC_COMMON) $(LIBS_CLIENT)

clean:
	del /f server.exe client.exe 2>NUL