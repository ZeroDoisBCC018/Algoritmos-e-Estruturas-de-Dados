all:
	gcc $(wildcard *.c *.h) -o Googlebot -std=c89 -Wall 
run:
	./Googlebot
