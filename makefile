CC=gcc
CFLAGS=-I.

mc2js: mc2js.c
	$(CC) -o mc2js mc2js.c $(CFLAGS)
