CFLAGS = -g -Wall -std=c11 -Iinclude

all : A2main

LinkedListAPI.o:
	gcc $(CFLAGS) -c src/LinkedListAPI.c -o bin/LinkedListAPI.o

list: LinkedListAPI.o
	ar cr bin/libllist.a bin/LinkedListAPI.o

CalendarParser.o:
	gcc $(CFLAGS) -c src/CalendarParser.c -o bin/CalendarParser.o -lllist

BasicFunctions.o:
	gcc $(CFLAGS) -c src/BasicFunctions.c -o bin/BasicFunctions.o -lllist

readFile.o:
	gcc $(CFLAGS) -c src/readFile.c -o bin/readFile.o -lllist

parser: CalendarParser.o BasicFunctions.o readFile.o
	ar cr bin/libcparse.a bin/CalendarParser.o bin/readFile.o bin/BasicFunctions.o

A2main: parser
	gcc src/a2main.c $(CFLAGS) -lncurses -lcparser -lllist -o bin/A2main

clean:
	rm bin/*.o bin/*.a A2main
