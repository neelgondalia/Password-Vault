CC          =	gcc

CFLAGS      =	-Wall -std=c99 -pedantic -g  

prog: makebin src/*.c
	$(CC) $(CFLAGS) src/main.c src/functions.c -lncurses -Iinclude -o bin/main

cppcheck:
	cppcheck --enable=all --language=c --std=c99 --inconclusive --suppress=missingInclude src/*.c -i ./include

doxy: Doxyfile
	doxygen
	
makebin:
	mkdir -p bin

clean:
	rm bin/*
