all: tp2virtual

tp2virtual: main.o substituicao.o
	gcc -o tp2virtual main.o substituicao.o

tp2virtual.o: main.c substituicao.c
	gcc -c main.c substituicao.c

clean:
	rm -f *.o *~ 
