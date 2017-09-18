CFLAGS = -Wall -std=c11 -Iinclude


all: list parser
	
LinkedListAPI.o:
	gcc $(CFLAGS) -c src/LinkedListAPI.c -o bin/LinkedListAPI.o

list: LinkedListAPI.o
	ar rcs bin/libllist.a bin/LinkedListAPI.o

CalendarParser.o:
	gcc $(CFLAGS) -c src/CalendarParser.c -o bin/CalendarParser.o

parser: CalendarParser.o
	ar rcs bin/libcparse.a bin/CalendarParser.o

clean:
	rm bin/libllist.a bin/libcparser.a bin/LinkedListAPI.o bin/CalendarParser.o
