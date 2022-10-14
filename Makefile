# Declaratiile de variabile
CC = gcc
CFLAGS = -g -Wall -lm -std=c11

# Regula de compilare
all: build

build: quadtree.c
	$(CC) -o quadtree quadtree.c $(CFLAGS)


# Regulile de "curatenie" (se folosesc pentru stergerea fisierelor intermediare\
si/sau rezultate)

clean:
	rm -f quadtree
	rm -f *.o


