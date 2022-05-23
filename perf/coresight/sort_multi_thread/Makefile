.PHONY: all normal clean

CC=gcc
C_FLAGS=-g -pthread -static


all: normal

normal: sort.c
	$(CC) sort.c $(C_FLAGS) -o sort

clean:
	rm sort
