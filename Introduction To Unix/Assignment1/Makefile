# Makefile
CC = gcc
CFLAGS = -Wall

all: atm

atm: main.o atm.o
	$(CC) $(CFLAGS) -o atm main.o atm.o

main.o: main.c atm.h
	$(CC) $(CFLAGS) -c main.c

atm.o: atm.c atm.h
	$(CC) $(CFLAGS) -c atm.c

clean:
	rm -f *.o atm
