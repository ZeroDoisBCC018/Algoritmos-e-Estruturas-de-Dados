all:
	gcc $(wildcard *.c *.h) -o Googlebot -std=c89 -Wall -g
run:
	./Googlebot
