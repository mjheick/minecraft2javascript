CC=gcc
CFLAGS=-I. -lm -lz

mc2js: mc2js.c
	$(CC) -o mc2js mc2js.c $(CFLAGS)
