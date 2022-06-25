ALL: main

main: main.o Tabuleiro.o Gameplay.o Jogadas.o util.o
	gcc main.o Tabuleiro.o Gameplay.o Jogadas.o util.o -o main

main.o: main.c estrutura.h
	gcc -c main.c

Tabuleiro.o: Tabuleiro.c estrutura.h
	gcc -c Tabuleiro.c

Gameplay.o: Gameplay.c estrutura.h
	gcc -c Gameplay.c

Jogadas.o: Jogadas.c estrutura.h
	gcc -c Jogadas.c

util.o: util.c estrutura.h
	gcc -c util.c

clean:
	rm -f *.o main