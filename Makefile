CC = g++

SRC = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp) $(wildcard lib/**/**/*.c)

LIBS = -l:libglfw3.a -opengl32 -lgdi32 -l:libassimp.a -lminizip -lz

CFLAGS = -Wall -Wextra -g3 -O2 -Ilib -Ilib/glad/include -Isrc

OUT = bin/main

all:
	$(CC) $(SRC) $(CFLAGS) $(LIBS) -o $(OUT)
