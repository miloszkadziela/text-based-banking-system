all: banking-system

banking-system: banking-system.o
	gcc -fsanitize=address -g $^ -o $@ -lm -lSDL -lpthread -lSDL_gfx

.c.o: 
	gcc -fsanitize=address -g -Wall -pedantic -std=c99 -c -D_REENTRANT $<

banking-system.o: banking-system.c

clean:
	-rm banking-system.o banking-system