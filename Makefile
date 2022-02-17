CC = g++

SRC = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp) $(wildcard lib/**/**/*.c)

LIBS = -l:libglfw3.a -opengl32 -lgdi32 -lassimp.dll
#-l:libassimp.a -lz

CFLAGS = -Wall -Wextra -g3 -O2 -Ilib -Ilib/glad/include -Isrc

OUT = bin/main

all:
	$(CC) $(SRC) $(CFLAGS) $(LIBS) -o $(OUT)
