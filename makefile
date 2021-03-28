PREFIX = ~/.local

all:
	gcc ./tty-gol.c -o ./tty-gol -Wall
install:
	mkdir -p ${PREFIX}/bin/
	cp ./tty-gol ${PREFIX}/bin/
