CC=gcc
CFLAGS=-lm -I.

mc2js: mc2js.c
	$(CC) -o mc2js mc2js.c $(CFLAGS)
