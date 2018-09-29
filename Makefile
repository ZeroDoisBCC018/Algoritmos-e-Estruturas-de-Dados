all:
	gcc $(wildcard *.c *.h) -o main -std=c89 -Wall

