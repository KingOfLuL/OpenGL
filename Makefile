CC = g++

SRC = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp) $(wildcard lib/**/**/*.c)

LIBS = -Llib -l:libassimp.dll.a -l:libglfw3.dll.a

CFLAGS = -Wall -Wextra -g3 -O2 -Ilib -Ilib/glad/include -Isrc

OUT = bin/main

all:
	$(CC) $(SRC) $(CFLAGS) $(LIBS) -o $(OUT)
