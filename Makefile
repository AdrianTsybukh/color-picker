CC = gcc
CFLAGS = -Wall -std=c17 $(shell pkg-config --cflags raylib)
LDFLAGS = $(shell pkg-config --libs raylib) -lm -lpthread -ldl -lrt -lX11

all: main

main: main.c
	$(CC) $(CFLAGS) -o main main.c $(LDFLAGS)
	./main

clean:
	rm -f main
